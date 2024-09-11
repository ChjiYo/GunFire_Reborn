#include "stdafx.h"

#include "Item_Coin.h"
#include "GameInstance.h"

#include "UI_Coin.h"

CItem_Coin::CItem_Coin(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CItem_Weapon{ pDevice , pContext }
{
}

CItem_Coin::CItem_Coin(const CItem_Coin& pPrototype)
	:CItem_Weapon{ pPrototype }
{
}

HRESULT CItem_Coin::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CItem_Coin::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	ITEM_DESC* Desc = static_cast<ITEM_DESC*>(pArg);
	m_tItem_Desc.pOriginTransform = Desc->pOriginTransform;
	
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_tItem_Desc.pOriginTransform->Get_State(CTransform::STATE_POSITION));

	if (FAILED(Ready_Components()))
		return E_FAIL;

	return S_OK;
}

void CItem_Coin::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);

}

void CItem_Coin::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);

	m_pCollider->Update(m_pTransformCom->Get_WorldMatrix());

	if(!m_bLand)
		m_bLand  = 	m_pTransformCom->Gravity(1.f, 0.f, fTimeDelta);

	m_pTransformCom->Turn({0 , 1, 0}, fTimeDelta);
	CTransform* pPlayerTransform =dynamic_cast<CTransform*>(m_pGameInstance->Get_Component(m_pGameInstance->GetCurrentLevelIndex(), TEXT("Layer_Player")  , TEXT("Com_Transform")));
	m_pTransformCom->RangeChase(fTimeDelta, pPlayerTransform->Get_State(CTransform::STATE_POSITION), 3.f);
}

void CItem_Coin::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);

	CCollider* pCollider = dynamic_cast<CCollider*>(m_pGameInstance->Get_Component(m_pGameInstance->GetCurrentLevelIndex(), TEXT("Layer_Player"), TEXT("Com_Collider")));
	if (m_pCollider->isCollision(pCollider))
	{
		dynamic_cast<CUI_Coin*>(m_pGameInstance->Get_Layer(m_pGameInstance->GetCurrentLevelIndex(), TEXT("Layer_UI_Player_UI_Coin")).back())->Set_Coin(rand() % 100 + 10);
		m_bDead = true;
	}

	m_pGameInstance->Add_RenderObject(CRenderer::RG_NONBLEND, this);


#ifdef _DEBUG
	m_pGameInstance->Add_DebugComponent(m_pCollider);
#endif
}

HRESULT CItem_Coin::Render()
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

HRESULT CItem_Coin::Ready_Components()
{
	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxMesh"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* For.Com_Model */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Item_Coin"),
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	/* For.Com_Collider */
	CBounding_Sphere::BOUNDING_SPHERE_DESC		SphereBoundingDesc{};

	SphereBoundingDesc.fRadius = 0.5f;
	SphereBoundingDesc.vCenter = _float3(0.f, SphereBoundingDesc.fRadius * 0.5f, 0.f);
	SphereBoundingDesc.pObject = this;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_Sphere"),
		TEXT("Com_ColliderSphere"), reinterpret_cast<CComponent**>(&m_pCollider), &SphereBoundingDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CItem_Coin::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;

	return S_OK;
}

CItem_Coin* CItem_Coin::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CItem_Coin* pInstance = new CItem_Coin(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CItem_Coin"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CItem_Coin::Clone(void* pArg)
{
	CItem_Coin* pInstance = new CItem_Coin(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CItem_Coin"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CItem_Coin::Free()
{
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pCollider);

	__super::Free();
}
