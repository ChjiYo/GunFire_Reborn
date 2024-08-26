#include "stdafx.h"

#include "FSM_Mon_Shield_Walk.h"
#include "GameInstance.h"

#include "Skill.h"
#include "Player.h"

CFSM_Mon_Shield_Walk::CFSM_Mon_Shield_Walk(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CFSM_Monster{ pDevice , pContext }
{
}

CFSM_Mon_Shield_Walk::CFSM_Mon_Shield_Walk(const CFSM_Mon_Shield_Walk& pPrototype)
	:CFSM_Monster{ pPrototype }
{
}

HRESULT CFSM_Mon_Shield_Walk::Initialize(FSM_MON_DESC Desc)
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	m_tDesc.pTransform = Desc.pTransform;
	m_tDesc.pNavigation = Desc.pNavigation;
	m_tDesc.pModel = Desc.pModel;

	return S_OK;
}

wstring CFSM_Mon_Shield_Walk::Priority_Update(_float fTimeDelta)
{
	return TEXT("Monster_Shield_State_Walk");
}

wstring CFSM_Mon_Shield_Walk::Update(_float fTimeDelta)
{
	return TEXT("Monster_Shield_State_Walk");
}

wstring CFSM_Mon_Shield_Walk::Late_Update(_float fTimeDelta)
{
	return TEXT("Monster_Shield_State_Walk");
}

void CFSM_Mon_Shield_Walk::Enter()
{
	__super::Enter();

	m_strName = TEXT("Monster_Shield_State_Walk");

}

void CFSM_Mon_Shield_Walk::Exit()
{
}

CFSM_Mon_Shield_Walk* CFSM_Mon_Shield_Walk::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, FSM_MON_DESC Desc)
{
	CFSM_Mon_Shield_Walk* pInstance = new CFSM_Mon_Shield_Walk(pDevice, pContext);

	if (FAILED(pInstance->Initialize(Desc)))
	{
		MSG_BOX(TEXT("Failed to Created : CFSM_Mon_Shield_Walk "));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CFSM_Mon_Shield_Walk::Free()
{
	__super::Free();
}
