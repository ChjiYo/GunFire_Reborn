#include "FSM_State.h"

#include "GameInstance.h"

CFSM_State::CFSM_State(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: m_pGameInstance{ CGameInstance::Get_Instance() }
{
	Safe_AddRef(m_pGameInstance);
}

CFSM_State::CFSM_State(const CFSM_State& Prototype)
	:m_pGameInstance{ Prototype.m_pGameInstance }
{
	Safe_AddRef(m_pGameInstance);
}

HRESULT CFSM_State::Initialize()
{
	return S_OK;
}

wstring CFSM_State::Priority_Update(_float fTimeDelta)
{
	return TEXT("");
}

wstring CFSM_State::Update(_float fTimeDelta)
{
	return TEXT("");
}

wstring CFSM_State::Late_Update(_float fTimeDelta)
{
	return TEXT("");
}


void CFSM_State::Enter()
{

}

void CFSM_State::Exit()
{
}

void CFSM_State::Free()
{
	Safe_Release(m_pGameInstance);

	__super::Free();
}
