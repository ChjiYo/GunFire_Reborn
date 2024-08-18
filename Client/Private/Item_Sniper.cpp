#include "stdafx.h"

#include "Item_Sniper.h"
#include "GameInstance.h"

CItem_Sniper::CItem_Sniper(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CItem_Weapon{ pDevice , pContext }
{
}

CItem_Sniper::CItem_Sniper(const CItem_Sniper& pPrototype)
	:CItem_Weapon{ pPrototype }
{
}

HRESULT CItem_Sniper::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CItem_Sniper::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, { 10.f, 0.5f , 10.f , 1.f });

	return S_OK;
}

void CItem_Sniper::Priority_Update(_float fTimeDelta)
{
}

void CItem_Sniper::Update(_float fTimeDelta)
{
}

void CItem_Sniper::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderObject(CRenderer::RG_NONBLEND, this);
}

HRESULT CItem_Sniper::Render()
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

HRESULT CItem_Sniper::Ready_Components()
{
	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxMesh"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* For.Com_Model */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Item_Weapon_Sniper"),
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CItem_Sniper::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;

	return S_OK;
}

CItem_Sniper* CItem_Sniper::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CItem_Sniper* pInstance = new CItem_Sniper(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CItem_Sniper"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CItem_Sniper::Clone(void* pArg)
{
	CItem_Sniper* pInstance = new CItem_Sniper(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CItem_Sniper"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CItem_Sniper::Free()
{
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModelCom);

	__super::Free();
}
