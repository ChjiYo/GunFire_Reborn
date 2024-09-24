#include "stdafx.h"

#include "FSM_Mon_Boom_Attack.h"
#include "GameInstance.h"
#include "Animation.h"

#include "Skill.h"
#include "Player.h"

CFSM_Mon_Boom_Attack::CFSM_Mon_Boom_Attack(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CFSM_Monster{ pDevice , pContext }
{
}

CFSM_Mon_Boom_Attack::CFSM_Mon_Boom_Attack(const CFSM_Mon_Boom_Attack& pPrototype)
	:CFSM_Monster{ pPrototype }
{
}

HRESULT CFSM_Mon_Boom_Attack::Initialize(FSM_MON_DESC Desc)
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	m_tDesc.pTransform = Desc.pTransform;
	m_tDesc.pNavigation = Desc.pNavigation;
	m_tDesc.pModel = Desc.pModel;
	m_tDesc.pOriginObj = Desc.pOriginObj;

	return S_OK;
}

wstring CFSM_Mon_Boom_Attack::Priority_Update(_float fTimeDelta)
{
	m_fAttackEndTime += fTimeDelta;

	return TEXT("Monster_Boom_State_Attack");
}

wstring CFSM_Mon_Boom_Attack::Update(_float fTimeDelta)
{
	if (m_fAttackEndTime >= m_tDesc.pModel->Get_Anim(3)->Get_Duration())
	{
		CSkill::SKILL_DESC Desc = {};
		Desc.pOriginTransform = m_tDesc.pTransform;
		Desc.iNumParts = CSkill::PART_END;

		m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Skill_Mon_Boom"), TEXT("Layer_Monster_Skill"),&Desc);
		m_tDesc.pOriginObj->Set_Dead();

		m_pGameInstance->Play_Sound(L"SFX_Mon_Boom", LEVEL_STATIC, false);
	}
	
	return TEXT("Monster_Boom_State_Attack");
}

wstring CFSM_Mon_Boom_Attack::Late_Update(_float fTimeDelta)
{
	return TEXT("Monster_Boom_State_Attack");
}

void CFSM_Mon_Boom_Attack::Enter()
{
	__super::Enter();

	m_strName = TEXT("Monster_Boom_State_Attack");
	m_tDesc.pModel->SetUp_Animation(3, false);

}

void CFSM_Mon_Boom_Attack::Exit()
{
}

CFSM_Mon_Boom_Attack* CFSM_Mon_Boom_Attack::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, FSM_MON_DESC Desc)
{
	CFSM_Mon_Boom_Attack* pInstance = new CFSM_Mon_Boom_Attack(pDevice, pContext);

	if (FAILED(pInstance->Initialize(Desc)))
	{
		MSG_BOX(TEXT("Failed to Created : CFSM_Mon_Boom_Attack "));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CFSM_Mon_Boom_Attack::Free()
{
	__super::Free();
}
