#include "stdafx.h"

#include "FSM_Mon_DragonHead_Attack.h"
#include "GameInstance.h"
#include "Animation.h"

#include "Skill.h"
#include "Player.h"

CFSM_Mon_DragonHead_Attack::CFSM_Mon_DragonHead_Attack(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CFSM_Monster{ pDevice , pContext }
{
}

CFSM_Mon_DragonHead_Attack::CFSM_Mon_DragonHead_Attack(const CFSM_Mon_DragonHead_Attack& pPrototype)
	:CFSM_Monster{ pPrototype }
{
}

HRESULT CFSM_Mon_DragonHead_Attack::Initialize(FSM_MON_DESC Desc)
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	m_tDesc.pTransform = Desc.pTransform;
	m_tDesc.pNavigation = Desc.pNavigation;
	m_tDesc.pModel = Desc.pModel;
	m_tDesc.tMonDesc = Desc.tMonDesc;
	m_tDesc.pOriginObj = Desc.pOriginObj;
	return S_OK;
}

wstring CFSM_Mon_DragonHead_Attack::Priority_Update(_float fTimeDelta)
{
	m_fAttackEndTimer += fTimeDelta;
	m_fSkillCreateTimer += fTimeDelta;
	
	return TEXT("Monster_DragonHead_State_Attack");
}

wstring CFSM_Mon_DragonHead_Attack::Update(_float fTimeDelta)
{
	if (!m_bSeccondAttack && m_tDesc.pModel->Get_Anim(11)->Get_Duration() <= m_fAttackEndTimer)
	{
		m_fAttackEndTimer = 0.f;
		m_bSeccondAttack = true;
		m_tDesc.pModel->SetUp_Animation(12, false);
	}

	if (!m_bAttackEnd && m_fSkillCreateTimer >= 1.5)
	{
		m_fSkillCreateTimer = 0.f;
		m_bAttackEnd = true;

		CSkill::SKILL_DESC	BubbleDesc = {};

		BubbleDesc.pOriginTransform = m_tDesc.pTransform;
		BubbleDesc.fSpeedPerSec = 10.f;
		BubbleDesc.fRotationPerSec = XMConvertToRadians(90.0f);
		BubbleDesc.iSkillDamage = 5.f;

		m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Skill_Dragon_Breath"), TEXT("Layer_Skill_Dragon_Breath"), &BubbleDesc);
	}



	return TEXT("Monster_DragonHead_State_Attack");
}

wstring CFSM_Mon_DragonHead_Attack::Late_Update(_float fTimeDelta)
{
	if (m_bSeccondAttack && m_tDesc.pModel->Get_Anim(12)->Get_Duration() <= m_fAttackEndTimer)
	{
		m_fAttackEndTimer = 0.f;
		m_bSeccondAttack = false;

		return TEXT("Monster_DragonHead_State_Idle");
	}

	return TEXT("Monster_DragonHead_State_Attack");
}

void CFSM_Mon_DragonHead_Attack::Enter()
{
	__super::Enter();


	m_tDesc.pModel->SetUp_Animation(11, false);
	m_strName = TEXT("Monster_DragonHead_State_Attack");
}

void CFSM_Mon_DragonHead_Attack::Exit()
{
}

CFSM_Mon_DragonHead_Attack* CFSM_Mon_DragonHead_Attack::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, FSM_MON_DESC Desc)
{
	CFSM_Mon_DragonHead_Attack* pInstance = new CFSM_Mon_DragonHead_Attack(pDevice, pContext);

	if (FAILED(pInstance->Initialize(Desc)))
	{
		MSG_BOX(TEXT("Failed to Created : CFSM_Mon_DragonHead_Attack "));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CFSM_Mon_DragonHead_Attack::Free()
{
	__super::Free();
}
