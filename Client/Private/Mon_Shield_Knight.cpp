#include "stdafx.h"
#include "..\Public\Mon_Shield_Knight.h"

#include "GameInstance.h"

#include "FSM_Mon_Shield_Idle.h"
#include "FSM_Mon_Shield_Hit.h"
#include "FSM_Mon_Shield_Walk.h"
#include "FSM_Mon_Shield_Run.h"
#include "FSM_Mon_Shield_Attack.h"

#include "PartObject.h"
#include "Shield.h"

CMon_Shield_Knight::CMon_Shield_Knight(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CMonster{ pDevice, pContext }
{

}

CMon_Shield_Knight::CMon_Shield_Knight(const CMon_Shield_Knight& Prototype)
	: CMonster{ Prototype }
{

}

HRESULT CMon_Shield_Knight::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CMon_Shield_Knight::Initialize(void* pArg)
{
	CContainerObject::CONTAINEROBJECT_DESC		ContainerDesc{};
	ContainerDesc.fRotationPerSec = XMConvertToRadians(90.0f);
	ContainerDesc.fSpeedPerSec = 2.5f;
	ContainerDesc.iNumParts = PART_END;

	if (FAILED(__super::Initialize(&ContainerDesc)))
		return E_FAIL;

	MON_DESC* MonDesc = static_cast<MON_DESC*>(pArg);
	m_tMonDesc.iMaxHp = MonDesc->iMaxHp;
	m_tMonDesc.iHp = MonDesc->iHp;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Ready_PartObject()))
		return E_FAIL;

	if (FAILED(Ready_State()))
		return E_FAIL;

	if (FAILED(Ready_UI(2.5f)))
		return E_FAIL;

	m_pStateCom->Set_State(TEXT("Monster_Shield_State_Idle"));

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, { 43.42 , 1.f, 46.72,1.f });
	m_pNavigationCom->Init_Index(m_pTransformCom);

	return S_OK;
}

void CMon_Shield_Knight::Priority_Update(_float fTimeDelta)
{
	m_pStateCom->Priority_Update(fTimeDelta);

	if (m_bNoise)
		m_fTimer += fTimeDelta;
}

void CMon_Shield_Knight::Update(_float fTimeDelta)
{
	if (!m_isMove)
		fTimeDelta = 0.f;

 	m_pModelCom->Play_Animation(fTimeDelta);

	__super::Update(fTimeDelta);

	for (int i = 0; i < COLL_END; ++i)
		m_pColliderCom[i]->Update(m_pTransformCom->Get_WorldMatrix());

	Destroy();

	m_pStateCom->Update(fTimeDelta);
}

void CMon_Shield_Knight::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);

	m_pGameInstance->Add_RenderObject(CRenderer::RG_NONBLEND, this);

	m_pStateCom->Late_Update(fTimeDelta);

#ifdef _DEBUG
	for (size_t i = 0; i < COLL_END; i++)
	{
		m_pGameInstance->Add_DebugComponent(m_pColliderCom[i]);
	}
#endif
}

HRESULT CMon_Shield_Knight::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	_uint		iNumMeshes = m_pModelCom->Get_NumMeshes();

	for (size_t i = 0; i < iNumMeshes; i++)
	{
		if (FAILED(m_pModelCom->Bind_MaterialSRV(m_pShaderCom, aiTextureType_DIFFUSE, "g_DiffuseTexture", 0)))
			return E_FAIL;

		m_pModelCom->Bind_BoneMatrices(m_pShaderCom, "g_BoneMatrices", i);

		if (FAILED(m_pShaderCom->Begin(5)))
			return E_FAIL;

		if (FAILED(m_pModelCom->Render(i)))
			return E_FAIL;
	}

	return S_OK;
}

void CMon_Shield_Knight::Destroy()
{
	if (m_tMonDesc.iHp <= 0)
	{
		m_pModelCom->SetUp_Animation(0, false);
		m_pTransformCom->Set_Speed(0);
		m_bNoise = true;
	}
	if (m_tMonDesc.iHp <= 0 && m_fTimer >= 1)
	{
		DropItem();
		m_bDead = true;
	}

}

HRESULT CMon_Shield_Knight::Ready_State()
{
	CFSM_Monster::FSM_MON_DESC Desc = {};
	Desc.pTransform = m_pTransformCom;
	Desc.pNavigation = m_pNavigationCom;
	Desc.pModel = m_pModelCom;

	m_pStateCom->Add_State(CFSM_Mon_Shield_Idle::Create(m_pDevice, m_pContext, Desc), TEXT("Monster_Shield_State_Idle"));
	m_pStateCom->Add_State(CFSM_Mon_Shield_Hit::Create(m_pDevice, m_pContext, Desc), TEXT("Monster_Shield_State_Hit"));
	m_pStateCom->Add_State(CFSM_Mon_Shield_Walk::Create(m_pDevice, m_pContext, Desc), TEXT("Monster_Shield_State_Walk"));
	m_pStateCom->Add_State(CFSM_Mon_Shield_Run::Create(m_pDevice, m_pContext, Desc), TEXT("Monster_Shield_State_Run"));
	m_pStateCom->Add_State(CFSM_Mon_Shield_Attack::Create(m_pDevice, m_pContext, Desc), TEXT("Monster_Shield_State_Attack"));

	return S_OK;
}

HRESULT CMon_Shield_Knight::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	/* Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxAnimMesh"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* Com_Model */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Monster_Shield_Knight"),
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	/* Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Noise"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	/* Com_State */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_State"),
		TEXT("Com_State"), reinterpret_cast<CComponent**>(&m_pStateCom))))
		return E_FAIL;

	/* For.Com_Collider */
	CBounding_AABB::BOUNDING_AABB_DESC	AABBBoundingDesc{};

	AABBBoundingDesc.vExtents = _float3(0.5f, 1.f, 0.5f);
	AABBBoundingDesc.vCenter = _float3(0.f, AABBBoundingDesc.vExtents.y, 0.f);
	AABBBoundingDesc.pObject = this;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_AABB"),
		TEXT("Com_ColliderAABB"), reinterpret_cast<CComponent**>(&m_pColliderCom[COLL_AABB]), &AABBBoundingDesc)))
		return E_FAIL;

	/* For.Com_Collider */
	CBounding_Sphere::BOUNDING_SPHERE_DESC		SphereBoundingDesc{};

	SphereBoundingDesc.fRadius = 0.5f;
	SphereBoundingDesc.vCenter = _float3(0.f, SphereBoundingDesc.fRadius + 1.f, 0.f);
	SphereBoundingDesc.pObject = this;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_Sphere"),
		TEXT("Com_ColliderSphere"), reinterpret_cast<CComponent**>(&m_pColliderCom[COLL_SPHERE]), &SphereBoundingDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CMon_Shield_Knight::Ready_PartObject()
{

	//m_szName = 0x000001e8c1dc5ea0 "Bip001 L Finger0"
	CShield::WEAPON_DESC Desc{};
	Desc.pParentMatrix = m_pTransformCom->Get_WorldMatrixPtr();
	Desc.pSocketBoneMatrix = m_pModelCom->Get_BoneMatrixPtr("Bip001 L Hand");

	__super::Add_PartObject(PART_WEAPON, TEXT("Prototype_GameObject_Shield"), &Desc);

	return S_OK;
}

HRESULT CMon_Shield_Knight::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_ShaderResources(m_pShaderCom, "g_NoiseTexture")))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_Timer", &m_fTimer, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_vCamPosition", &m_pGameInstance->Get_CamPosition_Float4(), sizeof(_float4))))
		return E_FAIL;


	return S_OK;
}

CMon_Shield_Knight* CMon_Shield_Knight::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CMon_Shield_Knight* pInstance = new CMon_Shield_Knight(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CMon_Shield_Knight"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CMon_Shield_Knight::Clone(void* pArg)
{
	CMon_Shield_Knight* pInstance = new CMon_Shield_Knight(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CMon_Shield_Knight"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMon_Shield_Knight::Free()
{
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pStateCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pNavigationCom);
	Safe_Release(m_pTextureCom);

	for (size_t i = 0; i < COLL_END; i++)
		Safe_Release(m_pColliderCom[i]);

	__super::Free();
}
