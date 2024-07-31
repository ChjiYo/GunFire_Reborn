#include "stdafx.h"

#include "FSM_Monster.h"
#include "GameInstance.h"

CFSM_Monster::CFSM_Monster(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CFSM_State{ pDevice , pContext }
{
}

CFSM_Monster::CFSM_Monster(const CFSM_Monster& pPrototype)
	:CFSM_State{ pPrototype }
{
}

HRESULT CFSM_Monster::Initialize()
{
	m_pPlayerTransform = dynamic_cast<CTransform*>(m_pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_Player"), TEXT("Com_Transform")));

	return S_OK;
}

wstring CFSM_Monster::Priority_Update(_float fTimeDelta)
{
	return TEXT("");
}

wstring CFSM_Monster::Update(_float fTimeDelta)
{
	return TEXT("");
}

wstring CFSM_Monster::Late_Update(_float fTimeDelta)
{
	return TEXT("");
}

void CFSM_Monster::Enter()
{

}

void CFSM_Monster::Exit()
{
}

void CFSM_Monster::Free()
{
	__super::Free();
}
