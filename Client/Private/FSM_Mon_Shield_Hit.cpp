#include "stdafx.h"

#include "FSM_Mon_Shield_Hit.h"
#include "GameInstance.h"

#include "Skill.h"
#include "Player.h"

CFSM_Mon_Shield_Hit::CFSM_Mon_Shield_Hit(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CFSM_Monster{ pDevice , pContext }
{
}

CFSM_Mon_Shield_Hit::CFSM_Mon_Shield_Hit(const CFSM_Mon_Shield_Hit& pPrototype)
	:CFSM_Monster{ pPrototype }
{
}

HRESULT CFSM_Mon_Shield_Hit::Initialize(FSM_MON_DESC Desc)
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	m_tDesc.pTransform = Desc.pTransform;
	m_tDesc.pNavigation = Desc.pNavigation;
	m_tDesc.pModel = Desc.pModel;

	return S_OK;
}

wstring CFSM_Mon_Shield_Hit::Priority_Update(_float fTimeDelta)
{
	return TEXT("");
}

wstring CFSM_Mon_Shield_Hit::Update(_float fTimeDelta)
{
	return TEXT("");
}

wstring CFSM_Mon_Shield_Hit::Late_Update(_float fTimeDelta)
{
	return TEXT("");
}

void CFSM_Mon_Shield_Hit::Enter()
{
	__super::Enter();

}

void CFSM_Mon_Shield_Hit::Exit()
{
}

CFSM_Mon_Shield_Hit* CFSM_Mon_Shield_Hit::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, FSM_MON_DESC Desc)
{
	CFSM_Mon_Shield_Hit* pInstance = new CFSM_Mon_Shield_Hit(pDevice, pContext);

	if (FAILED(pInstance->Initialize(Desc)))
	{
		MSG_BOX(TEXT("Failed to Created : CFSM_Mon_Shield_Hit "));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CFSM_Mon_Shield_Hit::Free()
{
	__super::Free();
}
