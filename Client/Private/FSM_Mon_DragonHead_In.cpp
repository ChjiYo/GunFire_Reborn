#include "stdafx.h"

#include "FSM_Mon_DragonHead_In.h"
#include "GameInstance.h"
#include "Animation.h"

#include "Skill.h"
#include "Player.h"

#include "VFX.h"

CFSM_Mon_DragonHead_In::CFSM_Mon_DragonHead_In(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CFSM_Monster{ pDevice , pContext }
{
}

CFSM_Mon_DragonHead_In::CFSM_Mon_DragonHead_In(const CFSM_Mon_DragonHead_In& pPrototype)
	:CFSM_Monster{ pPrototype }
{
}

HRESULT CFSM_Mon_DragonHead_In::Initialize(FSM_MON_DESC Desc)
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	m_tDesc.pTransform = Desc.pTransform;
	m_tDesc.pNavigation = Desc.pNavigation;
	m_tDesc.pModel = Desc.pModel;
	m_tDesc.tMonDesc = Desc.tMonDesc;

	return S_OK;
}

wstring CFSM_Mon_DragonHead_In::Priority_Update(_float fTimeDelta)
{
	m_fDelayTimer += fTimeDelta;

	return TEXT("Monster_DragonHead_State_In");
}

wstring CFSM_Mon_DragonHead_In::Update(_float fTimeDelta)
{
	return TEXT("Monster_DragonHead_State_In");
}

wstring CFSM_Mon_DragonHead_In::Late_Update(_float fTimeDelta)
{
	if (m_fDelayTimer >= m_tDesc.pModel->Get_Anim(4)->Get_Duration())
	{
		m_fDelayTimer = 0.f;
		return TEXT("Monster_Dragon_State_Idle");
	}

	return TEXT("Monster_DragonHead_State_In");
}

void CFSM_Mon_DragonHead_In::Enter()
{
	__super::Enter();

	m_tDesc.pTransform->LookAt(m_pPlayerTransform->Get_State(CTransform::STATE_POSITION));

	m_tDesc.pModel->SetUp_Animation(4, false);
	m_strName = TEXT("Monster_DragonHead_State_In");

	CVFX::VFX_DESC Desc = {};
	Desc.pOriginTransform = m_tDesc.pTransform;
	
	m_pGameInstance->Add_GameObject_ToLayer(LEVEL_BOSS, TEXT("Prototype_GameObject_VFX_In_Dust"), TEXT("Layer_Effect") , &Desc);
}

void CFSM_Mon_DragonHead_In::Exit()
{
}

CFSM_Mon_DragonHead_In* CFSM_Mon_DragonHead_In::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, FSM_MON_DESC Desc)
{
	CFSM_Mon_DragonHead_In* pInstance = new CFSM_Mon_DragonHead_In(pDevice, pContext);

	if (FAILED(pInstance->Initialize(Desc)))
	{
		MSG_BOX(TEXT("Failed to Created : CFSM_Mon_DragonHead_In"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CFSM_Mon_DragonHead_In::Free()
{
	__super::Free();
}
