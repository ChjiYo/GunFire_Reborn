#include "stdafx.h"

#include "FSM_Mon_Archer_Run.h"
#include "GameInstance.h"

#include "Skill.h"
#include "Player.h"

CFSM_Mon_Archer_Run::CFSM_Mon_Archer_Run(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CFSM_Monster{ pDevice , pContext }
{
}

CFSM_Mon_Archer_Run::CFSM_Mon_Archer_Run(const CFSM_Mon_Archer_Run& pPrototype)
	:CFSM_Monster{ pPrototype }
{
}

HRESULT CFSM_Mon_Archer_Run::Initialize()
{
	return S_OK;
}

wstring CFSM_Mon_Archer_Run::Priority_Update(_float fTimeDelta)
{
	return TEXT("");
}

wstring CFSM_Mon_Archer_Run::Update(_float fTimeDelta)
{
	return TEXT("");
}

wstring CFSM_Mon_Archer_Run::Late_Update(_float fTimeDelta)
{
	return TEXT("");
}

void CFSM_Mon_Archer_Run::Enter()
{
	__super::Enter();

}

void CFSM_Mon_Archer_Run::Exit()
{
}

CFSM_Mon_Archer_Run* CFSM_Mon_Archer_Run::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CFSM_Mon_Archer_Run* pInstance = new CFSM_Mon_Archer_Run(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX(TEXT("Failed to Created : CFSM_Mon_Archer_Run "));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CFSM_Mon_Archer_Run::Free()
{
	__super::Free();
}
