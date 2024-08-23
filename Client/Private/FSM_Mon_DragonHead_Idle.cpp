#include "stdafx.h"

#include "FSM_Mon_DragonHead_Idle.h"
#include "GameInstance.h"
#include "Animation.h"

#include "Skill.h"
#include "Player.h"

CFSM_Mon_DragonHead_Idle::CFSM_Mon_DragonHead_Idle(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CFSM_Monster{ pDevice , pContext }
{
}

CFSM_Mon_DragonHead_Idle::CFSM_Mon_DragonHead_Idle(const CFSM_Mon_DragonHead_Idle& pPrototype)
	:CFSM_Monster{ pPrototype }
{
}

HRESULT CFSM_Mon_DragonHead_Idle::Initialize(FSM_MON_DESC Desc)
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	m_tDesc.pTransform = Desc.pTransform;
	m_tDesc.pNavigation = Desc.pNavigation;
	m_tDesc.pModel = Desc.pModel;
	m_tDesc.pOriginObj = Desc.pOriginObj;

	return S_OK;
}

wstring CFSM_Mon_DragonHead_Idle::Priority_Update(_float fTimeDelta)
{
	m_fAttackDelay += fTimeDelta;

	return TEXT("Monster_DragonHead_State_Idle");
}

wstring CFSM_Mon_DragonHead_Idle::Update(_float fTimeDelta)
{
	m_tDesc.pTransform->LookAt(m_pPlayerTransform->Get_State(CTransform::STATE_POSITION));

	return TEXT("Monster_DragonHead_State_Idle");
}

wstring CFSM_Mon_DragonHead_Idle::Late_Update(_float fTimeDelta)
{
	if (m_fAttackDelay >= 2.f)
	{
		m_fAttackDelay = 0.f;
		return TEXT("Monster_DragonHead_State_Attack");
	}

	CMonster* pMoster = dynamic_cast<CMonster*>(m_tDesc.pOriginObj);

 	if(pMoster->Get_MonDesc().iHp / pMoster->Get_MonDesc().iMaxHp <= 0.5f)
		return TEXT("Monster_DragonHead_State_In");

	return TEXT("Monster_DragonHead_State_Idle");
}

void CFSM_Mon_DragonHead_Idle::Enter()
{
	__super::Enter();

	m_tDesc.pModel->SetUp_Animation(5, true);
	m_strName = TEXT("Monster_DragonHead_State_Idle");
}

void CFSM_Mon_DragonHead_Idle::Exit()
{
}

CFSM_Mon_DragonHead_Idle* CFSM_Mon_DragonHead_Idle::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, FSM_MON_DESC Desc)
{
	CFSM_Mon_DragonHead_Idle* pInstance = new CFSM_Mon_DragonHead_Idle(pDevice, pContext);

	if (FAILED(pInstance->Initialize(Desc)))
	{
		MSG_BOX(TEXT("Failed to Created : CFSM_Mon_DragonHead_Idle "));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CFSM_Mon_DragonHead_Idle::Free()
{
	__super::Free();
}
