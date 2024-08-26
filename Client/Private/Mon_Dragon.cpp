#include "stdafx.h"
#include "..\Public\Mon_Dragon.h"

#include "GameInstance.h"

#include "FSM_Mon_Dragon_Idle.h"
#include "FSM_Mon_Dragon_Attack.h"
#include "FSM_Mon_Dragon_Death.h"

#include "FSM_Mon_DragonHead_Idle.h"
#include "FSM_Mon_DragonHead_Attack.h"
#include "FSM_Mon_DragonHead_In.h"
#include "FSM_Mon_DragonHead_Out.h"

#include "FSM_Mon_DragonTail_Idle.h"
#include "FSM_Mon_DragonTail_Attack.h"
#include "FSM_Mon_DragonTail_In.h"
#include "FSM_Mon_DragonTail_Out.h"

#include "Player.h"

CMon_Dragon::CMon_Dragon(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CMonster{ pDevice, pContext }
{

}

CMon_Dragon::CMon_Dragon(const CMon_Dragon& Prototype)
	: CMonster{ Prototype }
{

}

HRESULT CMon_Dragon::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CMon_Dragon::Initialize(void* pArg)
{
	CContainerObject::CONTAINEROBJECT_DESC		ContainerDesc{};
	ContainerDesc.fRotationPerSec = XMConvertToRadians(45.0f);
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

	m_pStateCom->Set_State(TEXT("Monster_DragonHead_State_Idle"));

	m_pTransformCom->Set_Scaled(3.f, 3.f, 3.0f);
	m_pNavigationCom->Init_Index(m_pTransformCom);

	return S_OK;
}

void CMon_Dragon::Priority_Update(_float fTimeDelta)
{
	m_pStateCom->Priority_Update(fTimeDelta);
	m_pModelCom->Play_Animation(fTimeDelta);
}

void CMon_Dragon::Update(_float fTimeDelta)
{

	m_pColliderCom[COLL_AABB]->Update(m_pTransformCom->Get_WorldMatrix());

	Destroy();

	m_pStateCom->Update(fTimeDelta);
}

void CMon_Dragon::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);

	m_pGameInstance->Add_RenderObject(CRenderer::RG_NONBLEND, this);

	m_pStateCom->Late_Update(fTimeDelta);

#ifdef _DEBUG
	m_pGameInstance->Add_DebugComponent(m_pColliderCom[COLL_AABB]);

#endif
}

HRESULT CMon_Dragon::Render()
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

	return S_OK;
}

void CMon_Dragon::Destroy()
{
	if (m_tMonDesc.iHp <= 0.f)
		m_bDead = true;

}

HRESULT CMon_Dragon::Ready_State()
{
	CFSM_Monster::FSM_MON_DESC Desc = {};
	Desc.pTransform = m_pTransformCom;
	Desc.pNavigation = m_pNavigationCom;
	Desc.pModel = m_pModelCom;
	Desc.pOriginObj = this;

	m_pStateCom->Add_State(CFSM_Mon_Dragon_Idle::Create(m_pDevice, m_pContext, Desc), TEXT("Monster_Dragon_State_Idle"));
	m_pStateCom->Add_State(CFSM_Mon_Dragon_Attack::Create(m_pDevice, m_pContext, Desc), TEXT("Monster_Dragon_State_Attack"));
	m_pStateCom->Add_State(CFSM_Mon_Dragon_Death::Create(m_pDevice, m_pContext, Desc), TEXT("Monster_Dragon_State_Death"));
						   
	m_pStateCom->Add_State(CFSM_Mon_DragonHead_Idle::Create(m_pDevice, m_pContext, Desc), TEXT("Monster_DragonHead_State_Idle"));
	m_pStateCom->Add_State(CFSM_Mon_DragonHead_Attack::Create(m_pDevice, m_pContext, Desc), TEXT("Monster_DragonHead_State_Attack"));
	m_pStateCom->Add_State(CFSM_Mon_DragonHead_In::Create(m_pDevice, m_pContext, Desc), TEXT("Monster_DragonHead_State_In"));
	m_pStateCom->Add_State(CFSM_Mon_DragonHead_Out::Create(m_pDevice, m_pContext, Desc), TEXT("Monster_DragonHead_State_Out"));
						   
	m_pStateCom->Add_State(CFSM_Mon_DragonTail_Idle::Create(m_pDevice, m_pContext, Desc), TEXT("Monster_DragonTail_State_Idle"));
	m_pStateCom->Add_State(CFSM_Mon_DragonTail_Attack::Create(m_pDevice, m_pContext, Desc), TEXT("Monster_DragonTail_State_Attack"));
	m_pStateCom->Add_State(CFSM_Mon_DragonTail_In::Create(m_pDevice, m_pContext, Desc), TEXT("Monster_DragonTail_State_In"));
	m_pStateCom->Add_State(CFSM_Mon_DragonTail_Out::Create(m_pDevice, m_pContext, Desc), TEXT("Monster_DragonTail_State_Out"));

	return S_OK;
}

HRESULT CMon_Dragon::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	/* Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxAnimMesh"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* Com_Model */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Monster_Dragon"),
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	/* Com_State */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_State"),
		TEXT("Com_State"), reinterpret_cast<CComponent**>(&m_pStateCom))))
		return E_FAIL;

	/* For.Com_Collider */
	CBounding_AABB::BOUNDING_AABB_DESC	BoundingDesc{};

	BoundingDesc.vExtents = _float3(0.5f, 0.5f, 0.5f);
	BoundingDesc.vCenter = _float3(0.f, BoundingDesc.vExtents.y, 0.f);
	BoundingDesc.pObject = this;

	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_AABB"),
		TEXT("Com_ColliderAABB"), reinterpret_cast<CComponent**>(&m_pColliderCom[COLL_AABB]), &BoundingDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CMon_Dragon::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;

	return S_OK;
}

CMon_Dragon* CMon_Dragon::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CMon_Dragon* pInstance = new CMon_Dragon(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CMon_Dragon"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CMon_Dragon::Clone(void* pArg)
{
	CMon_Dragon* pInstance = new CMon_Dragon(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CMon_Dragon"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMon_Dragon::Free()
{
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pStateCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pNavigationCom);

	for (size_t i = 0; i < COLL_END; i++)
		Safe_Release(m_pColliderCom[i]);

	__super::Free();
}
