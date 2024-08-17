#include "stdafx.h"

#include "FSM_Mon_Knight_Attack.h"
#include "GameInstance.h"

#include "Skill.h"
#include "Player.h"

CFSM_Mon_Knight_Attack::CFSM_Mon_Knight_Attack(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CFSM_Monster{ pDevice , pContext }
{
}

CFSM_Mon_Knight_Attack::CFSM_Mon_Knight_Attack(const CFSM_Mon_Knight_Attack& pPrototype)
	:CFSM_Monster{ pPrototype }
{
}

HRESULT CFSM_Mon_Knight_Attack::Initialize(FSM_MON_DESC Desc)
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	m_tDesc.pTransform = Desc.pTransform;
	m_tDesc.pNavigation = Desc.pNavigation;
	m_tDesc.pModel = Desc.pModel;

	return S_OK;
}

wstring CFSM_Mon_Knight_Attack::Priority_Update(_float fTimeDelta)
{
	return TEXT("");
}

wstring CFSM_Mon_Knight_Attack::Update(_float fTimeDelta)
{
	return TEXT("");
}

wstring CFSM_Mon_Knight_Attack::Late_Update(_float fTimeDelta)
{
	return TEXT("");
}

void CFSM_Mon_Knight_Attack::Enter()
{
	__super::Enter();

}

void CFSM_Mon_Knight_Attack::Exit()
{
}

CFSM_Mon_Knight_Attack* CFSM_Mon_Knight_Attack::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, FSM_MON_DESC Desc)
{
	CFSM_Mon_Knight_Attack* pInstance = new CFSM_Mon_Knight_Attack(pDevice, pContext);

	if (FAILED(pInstance->Initialize(Desc)))
	{
		MSG_BOX(TEXT("Failed to Created : CFSM_Mon_Knight_Attack "));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CFSM_Mon_Knight_Attack::Free()
{
	__super::Free();
}
