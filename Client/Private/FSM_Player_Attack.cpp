#include "stdafx.h"

#include "FSM_Player_Attack.h"
#include "GameInstance.h"

#include "Skill.h"
#include "Player.h"

CFSM_Player_Attack::CFSM_Player_Attack(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CFSM_Player{ pDevice , pContext }
{
}

CFSM_Player_Attack::CFSM_Player_Attack(const CFSM_Player_Attack& pPrototype)
	:CFSM_Player{ pPrototype }
{
}

HRESULT CFSM_Player_Attack::Initialize()
{
	return S_OK;
}

wstring CFSM_Player_Attack::Priority_Update(_float fTimeDelta)
{
	return TEXT("");
}

wstring CFSM_Player_Attack::Update(_float fTimeDelta)
{
	return TEXT("");
}

wstring CFSM_Player_Attack::Late_Update(_float fTimeDelta)
{
	return TEXT("");
} 

void CFSM_Player_Attack::Enter()
{
	__super::Enter();

	//CSkill::SKILL_DESC	Desc = {};
	//Desc.pPlayerTransform = dynamic_cast<CTransform*>(m_pGameInstance->Get_Component(LEVEL_STATIC, TEXT("Layer_Player"), TEXT("Com_Transform")));
	//Desc.pCamTransform = dynamic_cast<CTransform*>(m_pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_Camera"), TEXT("Com_Transform")));
	//Desc.fSpeedPerSec = 5.f;
	//Desc.fRotationPerSec = XMConvertToRadians(90.0f);
	//
	//m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Skill_Bullet_Red"),TEXT("Layer_Skill_Player_Bullet"), &Desc);
}

void CFSM_Player_Attack::Exit()
{
}

CFSM_Player_Attack* CFSM_Player_Attack::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CFSM_Player_Attack* pInstance = new CFSM_Player_Attack(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX(TEXT("Failed to Created : CFSM_Player_Attack "));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CFSM_Player_Attack::Free()
{
	__super::Free();
}
