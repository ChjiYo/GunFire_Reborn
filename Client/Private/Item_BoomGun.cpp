#include "stdafx.h"

#include "Item_BoomGun.h"
#include "GameInstance.h"

CItem_BoomGun::CItem_BoomGun(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CItem_Weapon{ pDevice , pContext }
{
}

CItem_BoomGun::CItem_BoomGun(const CItem_BoomGun& pPrototype)
	:CItem_Weapon{ pPrototype }
{
}

HRESULT CItem_BoomGun::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CItem_BoomGun::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	ITEM_DESC* Desc = static_cast<ITEM_DESC*>(pArg);
	m_tItem_Desc.pOriginTransform = Desc->pOriginTransform;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_pTransformCom->Rotation({ 0.f, 1.f, 0.f }, XMConvertToRadians(90.f));

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_tItem_Desc.pOriginTransform->Get_State(CTransform::STATE_POSITION));
	m_eWeaponID = BOOMGUN;

	return S_OK;
}

void CItem_BoomGun::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);

	m_fMoveTimer += fTimeDelta;
}

void CItem_BoomGun::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);

	if(m_fMoveTimer <= 0.25f)
	{
		m_pTransformCom->Go_Up(fTimeDelta);
	}
}

void CItem_BoomGun::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
	m_pGameInstance->Add_RenderObject(CRenderer::RG_NONBLEND, this);
}

HRESULT CItem_BoomGun::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	_uint	iNumMeshes = m_pModelCom->Get_NumMeshes();

	for (size_t i = 0; i < iNumMeshes; i++)
	{
		if (FAILED(m_pModelCom->Bind_MaterialSRV(m_pShaderCom, aiTextureType_DIFFUSE, "g_DiffuseTexture", i)))
			return E_FAIL;

		m_pShaderCom->Begin(0);

		m_pModelCom->Render(i);
	}

	return S_OK;
}

HRESULT CItem_BoomGun::Ready_Components()
{
	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxMesh"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* For.Com_Model */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Item_Weapon_BoomGun"),
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CItem_BoomGun::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;

	return S_OK;
}

CItem_BoomGun* CItem_BoomGun::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CItem_BoomGun* pInstance = new CItem_BoomGun(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CItem_BoomGun"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CItem_BoomGun::Clone(void* pArg)
{
	CItem_BoomGun* pInstance = new CItem_BoomGun(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CItem_BoomGun"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CItem_BoomGun::Free()
{
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModelCom);

	__super::Free();
}
