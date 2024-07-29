#include "State_Machine.h"
#include "FSM_State.h"

CState_Machine::CState_Machine(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CComponent{ pDevice , pContext }
{
	Safe_AddRef(m_pCurState);
	Safe_AddRef(m_pPrevState);
}

CState_Machine::CState_Machine(const CState_Machine& Prototype)
	:CComponent{ Prototype }
{
	for (auto& Pair : m_pStates)
	{
		Safe_AddRef(Pair.second);
	}

	Safe_AddRef(m_pCurState);
	Safe_AddRef(m_pPrevState);
}

HRESULT CState_Machine::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CState_Machine::Initialize(void* pArg)
{
	return S_OK;
}

void CState_Machine::Priority_Update(_float fTimeDelta)
{
	if (nullptr == m_pCurState)
		return;

	const wstring& strStateName = m_pCurState->Priority_Update(fTimeDelta);

	if (strStateName != m_pCurState->Get_Name())
	{
		Change_State(strStateName);
		return;
	}

	return;
}

void CState_Machine::Update(_float fTimeDelta)
{
	if (nullptr == m_pCurState)
		return;

	const wstring& strStateName = m_pCurState->Update(fTimeDelta);

	if (strStateName != m_pCurState->Get_Name())
	{
		Change_State(strStateName);
		return;
	}

	return;
}

void CState_Machine::Late_Update(_float fTimeDelta)
{
	if (nullptr == m_pCurState)
		return;

	const wstring& strStateName = m_pCurState->Late_Update(fTimeDelta);

	if (strStateName != m_pCurState->Get_Name())
	{
		Change_State(strStateName);
		return;
	}

	return;
}

HRESULT CState_Machine::Set_State(const wstring& strStateTag)
{
	CFSM_State* pState = Find_State(strStateTag);

	if (nullptr == pState)
		return E_FAIL;

	return (Change_State(strStateTag));
}

HRESULT CState_Machine::Add_State(CFSM_State* pState, const wstring& strStateTag)
{
	if (nullptr == pState || Has_State(strStateTag))
		return E_FAIL;

	m_pStates.emplace(strStateTag, pState);
	
	Safe_AddRef(m_pCurState);

	return S_OK;
}

const _bool CState_Machine::Has_State(const wstring& strStateTag)
{
	auto iter = m_pStates.find(strStateTag);

	if (m_pStates.end() == iter)
		return false;

	return true;
}

CFSM_State* CState_Machine::Find_State(const wstring& strStateTag)
{
	auto iter = m_pStates.find(strStateTag);

	if (m_pStates.end() == iter)
		return nullptr;

	return iter->second;
}

HRESULT CState_Machine::Change_State(const wstring& strStateTag)
{
	CFSM_State* pNextState = Find_State(strStateTag);

	if (pNextState == nullptr)
		return E_FAIL;
	
	if (nullptr != m_pCurState)
	{
		m_pCurState->Exit();

	}
	
	m_pCurState = pNextState;

	m_pCurState->Initialize();
	m_pCurState->Enter();

	return S_OK;
}

CState_Machine* CState_Machine::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CState_Machine* pInstance = new CState_Machine(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CState_Machine"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CState_Machine::Clone(void* pArg)
{
	CState_Machine* pInstance = new CState_Machine(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CState_Machine"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Machine::Free()
{
	Safe_Release(m_pCurState);
	Safe_Release(m_pPrevState);

	for (auto& Pair : m_pStates)
	{
		Safe_Release(Pair.second);
	}

	m_pStates.clear();

	__super::Free();
}
