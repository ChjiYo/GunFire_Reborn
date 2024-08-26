#include "stdafx.h"

#include "FSM_Mon_Spear_Roll.h"
#include "GameInstance.h"

#include "Skill.h"
#include "Player.h"

CFSM_Mon_Spear_Roll::CFSM_Mon_Spear_Roll(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CFSM_Monster{ pDevice , pContext }
{
}

CFSM_Mon_Spear_Roll::CFSM_Mon_Spear_Roll(const CFSM_Mon_Spear_Roll& pPrototype)
	:CFSM_Monster{ pPrototype }
{
}

HRESULT CFSM_Mon_Spear_Roll::Initialize(FSM_MON_DESC Desc)
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	m_tDesc.pTransform = Desc.pTransform;
	m_tDesc.pNavigation = Desc.pNavigation;
	m_tDesc.pModel = Desc.pModel;

	return S_OK;
}

wstring CFSM_Mon_Spear_Roll::Priority_Update(_float fTimeDelta)
{
	return TEXT("");
}

wstring CFSM_Mon_Spear_Roll::Update(_float fTimeDelta)
{
	return TEXT("");
}

wstring CFSM_Mon_Spear_Roll::Late_Update(_float fTimeDelta)
{
	return TEXT("");
}

void CFSM_Mon_Spear_Roll::Enter()
{
	__super::Enter();

}

void CFSM_Mon_Spear_Roll::Exit()
{
}

CFSM_Mon_Spear_Roll* CFSM_Mon_Spear_Roll::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, FSM_MON_DESC Desc)
{
	CFSM_Mon_Spear_Roll* pInstance = new CFSM_Mon_Spear_Roll(pDevice, pContext);

	if (FAILED(pInstance->Initialize(Desc)))
	{
		MSG_BOX(TEXT("Failed to Created : CFSM_Mon_Spear_Roll "));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CFSM_Mon_Spear_Roll::Free()
{
	__super::Free();
}
