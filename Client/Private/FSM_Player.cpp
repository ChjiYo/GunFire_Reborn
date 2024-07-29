#include "stdafx.h"

#include "FSM_Player.h"
#include "GameInstance.h"

CFSM_Player::CFSM_Player(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CFSM_State{ pDevice , pContext }
{
}

CFSM_Player::CFSM_Player(const CFSM_Player& pPrototype)
	:CFSM_State{ pPrototype }
{
}

HRESULT CFSM_Player::Initialize()
{
	return S_OK;
}

wstring CFSM_Player::Priority_Update(_float fTimeDelta)
{
	return TEXT("");
}

wstring CFSM_Player::Update(_float fTimeDelta)
{
	return TEXT("");
}

wstring CFSM_Player::Late_Update(_float fTimeDelta)
{
	return TEXT("");
}

void CFSM_Player::Enter()
{

}

void CFSM_Player::Exit()
{
}

void CFSM_Player::Free()
{
	__super::Free();
}
