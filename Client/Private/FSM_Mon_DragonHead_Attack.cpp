#include "stdafx.h"

#include "FSM_Mon_DragonHead_Attack.h"
#include "GameInstance.h"
#include "Animation.h"

#include "Skill.h"
#include "Player.h"

#include "Skill_Dragon_Breath.h"

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
	m_fAttackDelay += fTimeDelta;

	return TEXT("Monster_DragonHead_State_Attack");
}

wstring CFSM_Mon_DragonHead_Attack::Update(_float fTimeDelta)
{
	if (m_bAttackEnable && m_fAttackDelay > 0.5f)
	{
		m_fAttackDelay = 0.f;

		CSkill::SKILL_DESC Skill_Desc = {};
		Skill_Desc.iNumSkill++;
		Skill_Desc.iSkillDamage = 5.f;
		Skill_Desc.pOriginTransform = m_tDesc.pTransform;
		
		m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Skill_Dragon_Breath"), TEXT("Layer_Skill_Dragon_Breath"), &Skill_Desc);
	}

	if (m_tDesc.pModel->Get_Anim(11)->Get_Duration() - 1.5f <= m_fAttackEndTimer)
	{
		m_tDesc.pModel->SetUp_Animation(12, false);
		m_bAttackEnable = false;
	}

	if (!m_bAttackEnable && m_tDesc.pModel->Get_Anim(12)->Get_Duration() - 1.5f <= m_fAttackEndTimer)
	{
		m_bAttackEnable = true;
		return TEXT("Monster_DragonHead_State_Idle");
	}

	return TEXT("Monster_DragonHead_State_Attack");
}

wstring CFSM_Mon_DragonHead_Attack::Late_Update(_float fTimeDelta)
{
	return TEXT("Monster_DragonHead_State_Attack");
}

void CFSM_Mon_DragonHead_Attack::Enter()
{
	__super::Enter();
	m_iNumSkill = 0;

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
