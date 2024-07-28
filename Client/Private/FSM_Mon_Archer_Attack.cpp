#include "stdafx.h"

#include "FSM_Mon_Archer_Attack.h"
#include "GameInstance.h"

#include "Skill.h"
#include "Player.h"

CFSM_Mon_Archer_Attack::CFSM_Mon_Archer_Attack(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CFSM_Player{ pDevice , pContext }
{
}

CFSM_Mon_Archer_Attack::CFSM_Mon_Archer_Attack(const CFSM_Mon_Archer_Attack& pPrototype)
	:CFSM_Player{ pPrototype }
{
}

HRESULT CFSM_Mon_Archer_Attack::Initialize()
{
	return S_OK;
}

void CFSM_Mon_Archer_Attack::Priority_Update(_float fTimeDelta)
{
}

void CFSM_Mon_Archer_Attack::Update(_float fTimeDelta)
{
}

void CFSM_Mon_Archer_Attack::Late_Update(_float fTimeDelta)
{
}

void CFSM_Mon_Archer_Attack::Enter()
{
	__super::Enter();

}

void CFSM_Mon_Archer_Attack::Exit()
{
}

CFSM_Mon_Archer_Attack* CFSM_Mon_Archer_Attack::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CFSM_Mon_Archer_Attack* pInstance = new CFSM_Mon_Archer_Attack(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
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
