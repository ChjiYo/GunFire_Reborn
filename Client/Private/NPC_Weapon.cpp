#include "stdafx.h"

#include "..\Public\NPC_Weapon.h"
#include "GameInstance.h"

#include "UIObject.h"

CNPC_Weapon::CNPC_Weapon(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CNPC{ pDevice, pContext }
{

}

CNPC_Weapon::CNPC_Weapon(const CNPC_Weapon& Prototype)
	: CNPC{ Prototype }
{

}

HRESULT CNPC_Weapon::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CNPC_Weapon::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, { 7.f , 0.f, 7.f, 1.f });

	m_pModelCom->SetUp_Animation(0, true);

	return S_OK;
}

void CNPC_Weapon::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);

}

void CNPC_Weapon::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);

	m_pModelCom->Play_Animation(fTimeDelta);

	m_pColliderCom->Update(m_pTransformCom->Get_WorldMatrix());
}

void CNPC_Weapon::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);

	CCollider* pPlayerCollider = dynamic_cast<CCollider*>(m_pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_Player"), TEXT("Com_Collider")));
	if (!m_bClick && m_pColliderCom->isCollision(pPlayerCollider) && m_pGameInstance->Key_Down('F'))
	{
		m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_NPC_Weapon_Panel"), TEXT("Layer_UI_ANPC_Weapon"));

		CUIObject::UI_DESC Desc = {};
		for (int i = 0; i < 2; i++)
		{
			Desc.iNumUI = i;

			m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_NPC_Weapon_Info"), TEXT("Layer_UI_ANPC_Weapon"), &Desc);
			m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_NPC_Weapon_Icon"), TEXT("Layer_UI_ANPC_Weapon"), &Desc);
			m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_NPC_Weapon_Btn"), TEXT("Layer_UI_ANPC_Weapon") , &Desc);
		}
		m_bClick = true;
		m_bCursorFree = true;
	}

	if (m_bClick && m_pColliderCom->isCollision(pPlayerCollider) && m_pGameInstance->Key_Down('F'))
	{
		for (auto& iter : m_pGameInstance->Get_Layer(LEVEL_GAMEPLAY, TEXT("Layer_UI_ANPC_Weapon")))
		{
			iter->Set_Dead();
		}
		m_bClick = false;
		m_bCursorFree = false;
	}

	m_pGameInstance->Add_RenderObject(CRenderer::RG_NONBLEND, this);

}

HRESULT CNPC_Weapon::Render()
{
	if(FAILED(Bind_ShaderResources()))
		return E_FAIL;

	_uint		iNumMeshes = m_pModelCom->Get_NumMeshes();

	for (size_t i = 0; i < iNumMeshes; i++)
	{
		if (FAILED(m_pModelCom->Bind_MaterialSRV(m_pShaderCom, aiTextureType_DIFFUSE, "g_DiffuseTexture", 0)))
			return E_FAIL;

		m_pModelCom->Bind_BoneMatrices(m_pShaderCom, "g_BoneMatrices", i);

		if (FAILED(m_pShaderCom->Begin(0)))
			return E_FAIL;

		if (FAILED(m_pModelCom->Render(i)))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CNPC_Weapon::Ready_Components()
{
	/* Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxAnimMesh"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* Com_Model */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_NPC_Weapon"),
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	/* For.Com_Collider */
	CBounding_Sphere::BOUNDING_SPHERE_DESC		SphereBoundingDesc{};

	SphereBoundingDesc.fRadius = 0.5f;
	SphereBoundingDesc.vCenter = _float3(0.f, SphereBoundingDesc.fRadius + 1.f, 0.f);
	SphereBoundingDesc.pObject = this;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_Sphere"),
		TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pColliderCom),&SphereBoundingDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CNPC_Weapon::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;

	return S_OK;
}


CNPC_Weapon* CNPC_Weapon::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CNPC_Weapon* pInstance = new CNPC_Weapon(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CNPC_Weapon"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CNPC_Weapon::Clone(void* pArg)
{
	CNPC_Weapon* pInstance = new CNPC_Weapon(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CNPC_Weapon"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CNPC_Weapon::Free()
{
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pColliderCom);

	__super::Free();
}
