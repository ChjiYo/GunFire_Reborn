#include "stdafx.h"

#include "FSM_Mon_Archer_Idle.h"
#include "GameInstance.h"

#include "Skill.h"
#include "Player.h"

CFSM_Mon_Archer_Idle::CFSM_Mon_Archer_Idle(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CFSM_Monster{ pDevice , pContext }
{
}

CFSM_Mon_Archer_Idle::CFSM_Mon_Archer_Idle(const CFSM_Mon_Archer_Idle& pPrototype)
	:CFSM_Monster{ pPrototype }
{
}

HRESULT CFSM_Mon_Archer_Idle::Initialize()
{
	return S_OK;
}

void CFSM_Mon_Archer_Idle::Priority_Update(_float fTimeDelta)
{
}

void CFSM_Mon_Archer_Idle::Update(_float fTimeDelta)
{
}

void CFSM_Mon_Archer_Idle::Late_Update(_float fTimeDelta)
{
}

void CFSM_Mon_Archer_Idle::Enter()
{
	__super::Enter();

}

void CFSM_Mon_Archer_Idle::Exit()
{
}

CFSM_Mon_Archer_Idle* CFSM_Mon_Archer_Idle::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CFSM_Mon_Archer_Idle* pInstance = new CFSM_Mon_Archer_Idle(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX(TEXT("Failed to Created : CFSM_Mon_Archer_Idle "));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CFSM_Mon_Archer_Idle::Free()
{
	__super::Free();
}
