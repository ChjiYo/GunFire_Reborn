#include "stdafx.h"

#include "FSM_Mon_Knight_RangeAttack.h"
#include "GameInstance.h"

#include "Skill.h"
#include "Player.h"

CFSM_Mon_Knight_RangeAttack::CFSM_Mon_Knight_RangeAttack(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CFSM_Monster{ pDevice , pContext }
{
}

CFSM_Mon_Knight_RangeAttack::CFSM_Mon_Knight_RangeAttack(const CFSM_Mon_Knight_RangeAttack& pPrototype)
	:CFSM_Monster{ pPrototype }
{
}

HRESULT CFSM_Mon_Knight_RangeAttack::Initialize(FSM_MON_DESC Desc)
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	m_tDesc.pTransform = Desc.pTransform;
	m_tDesc.pNavigation = Desc.pNavigation;
	m_tDesc.pModel = Desc.pModel;

	return S_OK;
}

wstring CFSM_Mon_Knight_RangeAttack::Priority_Update(_float fTimeDelta)
{
	return TEXT("");
}

wstring CFSM_Mon_Knight_RangeAttack::Update(_float fTimeDelta)
{
	return TEXT("");
}

wstring CFSM_Mon_Knight_RangeAttack::Late_Update(_float fTimeDelta)
{
	return TEXT("");
}

void CFSM_Mon_Knight_RangeAttack::Enter()
{
	__super::Enter();

}

void CFSM_Mon_Knight_RangeAttack::Exit()
{
}

CFSM_Mon_Knight_RangeAttack* CFSM_Mon_Knight_RangeAttack::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, FSM_MON_DESC Desc)
{
	CFSM_Mon_Knight_RangeAttack* pInstance = new CFSM_Mon_Knight_RangeAttack(pDevice, pContext);

	if (FAILED(pInstance->Initialize(Desc)))
	{
		MSG_BOX(TEXT("Failed to Created : CFSM_Mon_Knight_RangeAttack "));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CFSM_Mon_Knight_RangeAttack::Free()
{
	__super::Free();
}
