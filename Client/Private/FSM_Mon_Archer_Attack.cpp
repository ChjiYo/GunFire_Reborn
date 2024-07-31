#include "stdafx.h"

#include "FSM_Mon_Archer_Attack.h"
#include "GameInstance.h"

#include "Skill.h"
#include "Player.h"

CFSM_Mon_Archer_Attack::CFSM_Mon_Archer_Attack(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CFSM_Monster{ pDevice , pContext }
{
}

CFSM_Mon_Archer_Attack::CFSM_Mon_Archer_Attack(const CFSM_Mon_Archer_Attack& pPrototype)
	:CFSM_Monster{ pPrototype }
{
}

HRESULT CFSM_Mon_Archer_Attack::Initialize(FSM_MON_DESC Desc)
{
	m_tDesc.pTransform = Desc.pTransform;
	m_tDesc.pNavigation = Desc.pNavigation;
	m_tDesc.pModel = Desc.pModel;


	return S_OK;
}

wstring CFSM_Mon_Archer_Attack::Priority_Update(_float fTimeDelta)
{
	return TEXT("Monster_Archer_State_Attack");
}

wstring CFSM_Mon_Archer_Attack::Update(_float fTimeDelta)
{
	return TEXT("Monster_Archer_State_Attack");
}

wstring CFSM_Mon_Archer_Attack::Late_Update(_float fTimeDelta)
{
	return TEXT("Monster_Archer_State_Attack");
}

void CFSM_Mon_Archer_Attack::Enter()
{
	m_tDesc.pModel->SetUp_Animation( 2, false);

	__super::Enter();
}

void CFSM_Mon_Archer_Attack::Exit()
{
}

CFSM_Mon_Archer_Attack* CFSM_Mon_Archer_Attack::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext ,FSM_MON_DESC Desc)
{
	CFSM_Mon_Archer_Attack* pInstance = new CFSM_Mon_Archer_Attack(pDevice, pContext);

	if (FAILED(pInstance->Initialize(Desc)))
	{
		MSG_BOX(TEXT("Failed to Created : CFSM_Mon_Archer_Idle "));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CFSM_Mon_Archer_Attack::Free()
{
	__super::Free();
}
