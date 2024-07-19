#include "stdafx.h"

#include "FSM_Player_Jump.h"
#include "GameInstance.h"

CFSM_Player_Jump::CFSM_Player_Jump(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CFSM_Player{ pDevice , pContext }
{
}

CFSM_Player_Jump::CFSM_Player_Jump(const CFSM_Player_Jump& pPrototype)
	:CFSM_Player{ pPrototype }
{
}

HRESULT CFSM_Player_Jump::Initialize()
{
	return S_OK;
}

void CFSM_Player_Jump::Priority_Update(_float fTimeDelta)
{
}

void CFSM_Player_Jump::Update(_float fTimeDelta)
{
}

void CFSM_Player_Jump::Late_Update(_float fTimeDelta)
{
}

void CFSM_Player_Jump::Enter()
{
}

void CFSM_Player_Jump::Exit()
{
}

CFSM_Player_Jump* CFSM_Player_Jump::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CFSM_Player_Jump* pInstance = new CFSM_Player_Jump(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX(TEXT("Failed to Created : CFSM_Player_Jump "));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CFSM_Player_Jump::Free()
{
	__super::Free();
}
