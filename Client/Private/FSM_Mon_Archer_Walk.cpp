#include "stdafx.h"

#include "FSM_Mon_Archer_Walk.h"
#include "GameInstance.h"

#include "Skill.h"
#include "Player.h"

CFSM_Mon_Archer_Walk::CFSM_Mon_Archer_Walk(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CFSM_Monster{ pDevice , pContext }
{
}

CFSM_Mon_Archer_Walk::CFSM_Mon_Archer_Walk(const CFSM_Mon_Archer_Walk& pPrototype)
	:CFSM_Monster{ pPrototype }
{
}

HRESULT CFSM_Mon_Archer_Walk::Initialize(FSM_MON_DESC tDesc)
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	return S_OK;
}

wstring CFSM_Mon_Archer_Walk::Priority_Update(_float fTimeDelta)
{
	return TEXT("");
}

wstring CFSM_Mon_Archer_Walk::Update(_float fTimeDelta)
{
	return TEXT("");
}

wstring CFSM_Mon_Archer_Walk::Late_Update(_float fTimeDelta)
{
	return TEXT("");
}

void CFSM_Mon_Archer_Walk::Enter()
{
	__super::Enter();

}

void CFSM_Mon_Archer_Walk::Exit()
{
}

CFSM_Mon_Archer_Walk* CFSM_Mon_Archer_Walk::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, FSM_MON_DESC tDesc)
{
	CFSM_Mon_Archer_Walk* pInstance = new CFSM_Mon_Archer_Walk(pDevice, pContext);

	if (FAILED(pInstance->Initialize(tDesc)))
	{
		MSG_BOX(TEXT("Failed to Created : CFSM_Mon_Archer_Walk "));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CFSM_Mon_Archer_Walk::Free()
{
	__super::Free();
}
