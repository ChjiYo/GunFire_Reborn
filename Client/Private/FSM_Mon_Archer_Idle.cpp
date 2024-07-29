#include "stdafx.h"

#include "FSM_Mon_Archer_Idle.h"
#include "GameInstance.h"

#include "Skill.h"
#include "Player.h"

CFSM_Mon_Archer_Idle::CFSM_Mon_Archer_Idle(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CFSM_Monster{ pDevice , pContext }
{
}

CFSM_Mon_Archer_Idle::CFSM_Mon_Archer_Idle(const CFSM_Mon_Archer_Idle& pPrototype)
	:CFSM_Monster{ pPrototype }
{
}

HRESULT CFSM_Mon_Archer_Idle::Initialize()
{
	return S_OK;
}

wstring CFSM_Mon_Archer_Idle::Priority_Update(_float fTimeDelta)
{
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(m_pGameInstance->Get_Layer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Player")).front());
	_vector vPos = pPlayer->Get_Transform()->Get_State(CTransform::STATE_POSITION);
	

	return TEXT("");
}

wstring CFSM_Mon_Archer_Idle::Update(_float fTimeDelta)
{
	return TEXT("");
}

wstring CFSM_Mon_Archer_Idle::Late_Update(_float fTimeDelta)
{
	return TEXT("");
}

void CFSM_Mon_Archer_Idle::Enter()
{
	__super::Enter();

	m_strName = TEXT("Monster_Archer_State_Idle");
}

void CFSM_Mon_Archer_Idle::Exit()
{
}

CFSM_Mon_Archer_Idle* CFSM_Mon_Archer_Idle::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CFSM_Mon_Archer_Idle* pInstance = new CFSM_Mon_Archer_Idle(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX(TEXT("Failed to Created : CFSM_Mon_Archer_Idle "));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CFSM_Mon_Archer_Idle::Free()
{
	__super::Free();
}
