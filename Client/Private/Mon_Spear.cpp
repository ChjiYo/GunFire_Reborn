#include "stdafx.h"
#include "..\Public\Mon_Spear.h"

#include "GameInstance.h"

#include "FSM_Mon_Archer_Idle.h"
#include "FSM_Mon_Archer_Hit.h"
#include "FSM_Mon_Archer_Run.h"
#include "FSM_Mon_Archer_Walk.h"
#include "FSM_Mon_Archer_Attack.h"

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

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Ready_State()))
		return E_FAIL;

	if (FAILED(Ready_UI()))
		return E_FAIL;

	//m_pStateCom->Set_State(TEXT("Monster_Archer_State_Idle"));

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(20.f, 1.f, 20.f, 1.f));
	m_pNavigationCom->Init_Index(m_pTransformCom);

	m_pModelCom->SetUp_Animation(1, true);

	return S_OK;
}

void CMon_Spear::Priority_Update(_float fTimeDelta)
{
	//m_pStateCom->Priority_Update(fTimeDelta);
}

void CMon_Spear::Update(_float fTimeDelta)
{
	for (int i = 0; i < COLL_END; ++i)
		m_pColliderCom[i]->Update(m_pTransformCom->Get_WorldMatrix());


	Destroy();
}

void CMon_Spear::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);

	m_pModelCom->Play_Animation(fTimeDelta);
	m_pGameInstance->Add_RenderObject(CRenderer::RG_NONBLEND, this);

	//m_pStateCom->Late_Update(fTimeDelta);

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

		if (FAILED(m_pShaderCom->Begin(0)))
			return E_FAIL;

		if (FAILED(m_pModelCom->Render(i)))
			return E_FAIL;
	}

	wstring m_strMaxHp = to_wstring(m_iAnimFrame);
	m_pGameInstance->Draw_Font(TEXT("Font_Nexon"), m_strMaxHp, _float2(1200.f, 100.f), XMVectorSet(0.7f, 1.f, 0.3f, 1.f), XMConvertToRadians(-5.f), 0.8f);

	return S_OK;
}

void CMon_Spear::Destroy()
{
	if (m_tMonDesc.iHp <= 0.f)
		m_bDead = true;

}

HRESULT CMon_Spear::Ready_State()
{
	CFSM_Monster::FSM_MON_DESC Desc = {};
	Desc.pTransform = m_pTransformCom;
	Desc.pNavigation = m_pNavigationCom;
	Desc.pModel = m_pModelCom;

	//m_pStateCom->Add_State(CFSM_Mon_Archer_Idle::Create(m_pDevice, m_pContext, Desc), TEXT("Monster_Archer_State_Idle"));
	//m_pStateCom->Add_State(CFSM_Mon_Archer_Hit::Create(m_pDevice, m_pContext, Desc), TEXT("Monster_Archer_State_Hit"));
	//m_pStateCom->Add_State(CFSM_Mon_Archer_Walk::Create(m_pDevice, m_pContext, Desc), TEXT("Monster_Archer_State_Walk"));
	//m_pStateCom->Add_State(CFSM_Mon_Archer_Run::Create(m_pDevice, m_pContext, Desc), TEXT("Monster_Archer_State_Run"));
	//m_pStateCom->Add_State(CFSM_Mon_Archer_Attack::Create(m_pDevice, m_pContext, Desc), TEXT("Monster_Archer_State_Attack"));

	return S_OK;
}

HRESULT CMon_Spear::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	/* Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxAnimMesh"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* Com_Model */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Monster_Spear"),
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	/* Com_State */
	/*if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_State"),
		TEXT("Com_State"), reinterpret_cast<CComponent**>(&m_pStateCom))))
		return E_FAIL;*/

		/* For.Com_Collider */
	CBounding_AABB::BOUNDING_AABB_DESC	AABBBoundingDesc{};

	AABBBoundingDesc.vExtents = _float3(0.5f, 0.5f, 0.5f);
	AABBBoundingDesc.vCenter = _float3(0.f, AABBBoundingDesc.vExtents.y, 0.f);
	AABBBoundingDesc.pObject = this;

	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_AABB"),
		TEXT("Com_ColliderAABB"), reinterpret_cast<CComponent**>(&m_pColliderCom[COLL_AABB]), &AABBBoundingDesc)))
		return E_FAIL;

	/* For.Com_Collider */
	CBounding_Sphere::BOUNDING_SPHERE_DESC		SphereBoundingDesc{};

	SphereBoundingDesc.fRadius = 0.5f;
	SphereBoundingDesc.vCenter = _float3(0.f, SphereBoundingDesc.fRadius + 1.f, 0.f);
	SphereBoundingDesc.pObject = this;

	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_Sphere"),
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
	//Safe_Release(m_pStateCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pNavigationCom);

	for (size_t i = 0; i < COLL_END; i++)
		Safe_Release(m_pColliderCom[i]);

	__super::Free();
}
