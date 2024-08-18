#include "stdafx.h"

#include "FSM_Mon_Spear_Dash.h"
#include "GameInstance.h"

#include "Skill.h"
#include "Player.h"

CFSM_Mon_Spear_Dash::CFSM_Mon_Spear_Dash(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CFSM_Monster{ pDevice , pContext }
{
}

CFSM_Mon_Spear_Dash::CFSM_Mon_Spear_Dash(const CFSM_Mon_Spear_Dash& pPrototype)
	:CFSM_Monster{ pPrototype }
{
}

HRESULT CFSM_Mon_Spear_Dash::Initialize(FSM_MON_DESC Desc)
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	m_tDesc.pTransform = Desc.pTransform;
	m_tDesc.pNavigation = Desc.pNavigation;
	m_tDesc.pModel = Desc.pModel;

	return S_OK;
}

wstring CFSM_Mon_Spear_Dash::Priority_Update(_float fTimeDelta)
{
	return TEXT("");
}

wstring CFSM_Mon_Spear_Dash::Update(_float fTimeDelta)
{
	return TEXT("");
}

wstring CFSM_Mon_Spear_Dash::Late_Update(_float fTimeDelta)
{
	return TEXT("");
}

void CFSM_Mon_Spear_Dash::Enter()
{
	__super::Enter();

}

void CFSM_Mon_Spear_Dash::Exit()
{
}

CFSM_Mon_Spear_Dash* CFSM_Mon_Spear_Dash::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, FSM_MON_DESC Desc)
{
	CFSM_Mon_Spear_Dash* pInstance = new CFSM_Mon_Spear_Dash(pDevice, pContext);

	if (FAILED(pInstance->Initialize(Desc)))
	{
		MSG_BOX(TEXT("Failed to Created : CFSM_Mon_Spear_Dash "));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CFSM_Mon_Spear_Dash::Free()
{
	__super::Free();
}
