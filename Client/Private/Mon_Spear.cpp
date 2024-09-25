#include "stdafx.h"
#include "..\Public\Mon_Spear.h"

#include "GameInstance.h"

#include "FSM_Mon_Spear_Idle.h"
#include "FSM_Mon_Spear_Hit.h"
#include "FSM_Mon_Spear_Run.h"
#include "FSM_Mon_Spear_Walk.h"
#include "FSM_Mon_Spear_Attack.h"
#include "FSM_Mon_Spear_Roll.h"
#include "FSM_Mon_Spear_Dash.h"

CMon_Spear::CMon_Spear(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CMonster{ pDevice, pContext }
{

}

CMon_Spear::CMon_Spear(const CMon_Spear& Prototype)
	: CMonster{ Prototype }
{

}

HRESULT CMon_Spear::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CMon_Spear::Initialize(void* pArg)
{
	CContainerObject::CONTAINEROBJECT_DESC		ContainerDesc{};
	ContainerDesc.fRotationPerSec = XMConvertToRadians(90.0f);
	ContainerDesc.fSpeedPerSec = 2.5f;
	ContainerDesc.iNumParts = PART_HEAD;

	if (FAILED(__super::Initialize(&ContainerDesc)))
		return E_FAIL;

	MON_DESC* MonDesc = static_cast<MON_DESC*>(pArg);
	m_tMonDesc.iMaxHp = MonDesc->iMaxHp;
	m_tMonDesc.iHp = MonDesc->iHp;
	m_tMonDesc.iNum = MonDesc->iNum;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Ready_State()))
		return E_FAIL;

	if (FAILED(Ready_UI()))
		return E_FAIL;

	m_pStateCom->Set_State(TEXT("Monster_Spear_State_Idle"));

	switch (m_tMonDesc.iNum)
	{
	case 0:
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, {54.55f , 1.f, 40.61f, 1.f});
		break;

	case 1:
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, { 43.28f , 1.f, 40.61, 1.f });
		break;
	}

	m_pNavigationCom->Init_Index(m_pTransformCom);

	return S_OK;
}

void CMon_Spear::Priority_Update(_float fTimeDelta)
{
	m_pStateCom->Priority_Update(fTimeDelta);

	if (m_bNoise)
		m_fTimer += fTimeDelta;
}

void CMon_Spear::Update(_float fTimeDelta)
{ 
	if (!m_isMove)
		fTimeDelta = 0.f;

	__super::Update(fTimeDelta);

	m_pModelCom->Play_Animation(fTimeDelta);

	for (int i = 0; i < COLL_END; ++i)
		m_pColliderCom[i]->Update(m_pTransformCom->Get_WorldMatrix());


	Destroy();

	m_pStateCom->Update(fTimeDelta);
}

void CMon_Spear::Late_Update(_float fTimeDelta)
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

HRESULT CMon_Spear::Render()
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

void CMon_Spear::Destroy()
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

HRESULT CMon_Spear::Ready_State()
{
	CFSM_Monster::FSM_MON_DESC Desc = {};
	Desc.pTransform = m_pTransformCom;
	Desc.pNavigation = m_pNavigationCom;
	Desc.pModel = m_pModelCom;

	m_pStateCom->Add_State(CFSM_Mon_Spear_Idle::Create(m_pDevice, m_pContext, Desc), TEXT("Monster_Spear_State_Idle"));
	m_pStateCom->Add_State(CFSM_Mon_Spear_Hit::Create(m_pDevice, m_pContext, Desc), TEXT("Monster_Spear_State_Hit"));
	m_pStateCom->Add_State(CFSM_Mon_Spear_Walk::Create(m_pDevice, m_pContext, Desc), TEXT("Monster_Spear_State_Walk"));
	m_pStateCom->Add_State(CFSM_Mon_Spear_Run::Create(m_pDevice, m_pContext, Desc), TEXT("Monster_Spear_State_Run"));
	m_pStateCom->Add_State(CFSM_Mon_Spear_Attack::Create(m_pDevice, m_pContext, Desc), TEXT("Monster_Spear_State_Attack"));
	m_pStateCom->Add_State(CFSM_Mon_Spear_Roll::Create(m_pDevice, m_pContext, Desc), TEXT("Monster_Spear_State_Roll"));
	m_pStateCom->Add_State(CFSM_Mon_Spear_Dash::Create(m_pDevice, m_pContext, Desc), TEXT("Monster_Spear_State_Dash"));

	return S_OK;
}

HRESULT CMon_Spear::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	/* Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxAnimMesh"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* Com_Model */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Monster_Spear"),
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	/* Com_State */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_State"),
		TEXT("Com_State"), reinterpret_cast<CComponent**>(&m_pStateCom))))
		return E_FAIL;

	/* Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Noise"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

		/* For.Com_Collider */
	CBounding_AABB::BOUNDING_AABB_DESC	AABBBoundingDesc{};

	AABBBoundingDesc.vExtents = _float3(0.5f, 0.5f, 0.5f);
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

HRESULT CMon_Spear::Bind_ShaderResources()
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

CMon_Spear* CMon_Spear::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CMon_Spear* pInstance = new CMon_Spear(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CMon_Spear"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CMon_Spear::Clone(void* pArg)
{
	CMon_Spear* pInstance = new CMon_Spear(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CMon_Spear"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMon_Spear::Free()
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
