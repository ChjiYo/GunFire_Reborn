#include "stdafx.h"

#include "FSM_Mon_Knight_Run.h"
#include "GameInstance.h"

#include "Skill.h"
#include "Player.h"

CFSM_Mon_Knight_Run::CFSM_Mon_Knight_Run(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CFSM_Monster{ pDevice , pContext }
{
}

CFSM_Mon_Knight_Run::CFSM_Mon_Knight_Run(const CFSM_Mon_Knight_Run& pPrototype)
	:CFSM_Monster{ pPrototype }
{
}

HRESULT CFSM_Mon_Knight_Run::Initialize(FSM_MON_DESC Desc)
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	m_tDesc.pTransform = Desc.pTransform;
	m_tDesc.pNavigation = Desc.pNavigation;
	m_tDesc.pModel = Desc.pModel;

	return S_OK;
}

wstring CFSM_Mon_Knight_Run::Priority_Update(_float fTimeDelta)
{
	return TEXT("");
}

wstring CFSM_Mon_Knight_Run::Update(_float fTimeDelta)
{
	return TEXT("");
}

wstring CFSM_Mon_Knight_Run::Late_Update(_float fTimeDelta)
{
	return TEXT("");
}

void CFSM_Mon_Knight_Run::Enter()
{
	__super::Enter();

}

void CFSM_Mon_Knight_Run::Exit()
{
}

CFSM_Mon_Knight_Run* CFSM_Mon_Knight_Run::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, FSM_MON_DESC Desc)
{
	CFSM_Mon_Knight_Run* pInstance = new CFSM_Mon_Knight_Run(pDevice, pContext);

	if (FAILED(pInstance->Initialize(Desc)))
	{
		MSG_BOX(TEXT("Failed to Created : CFSM_Mon_Knight_Run "));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CFSM_Mon_Knight_Run::Free()
{
	__super::Free();
}
