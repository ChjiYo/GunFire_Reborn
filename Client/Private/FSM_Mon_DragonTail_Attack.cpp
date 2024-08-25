#include "stdafx.h"

#include "FSM_Mon_DragonTail_Attack.h"
#include "GameInstance.h"
#include "Animation.h"

#include "Skill.h"
#include "Player.h"

CFSM_Mon_DragonTail_Attack::CFSM_Mon_DragonTail_Attack(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CFSM_Monster{ pDevice , pContext }
{
}

CFSM_Mon_DragonTail_Attack::CFSM_Mon_DragonTail_Attack(const CFSM_Mon_DragonTail_Attack& pPrototype)
	:CFSM_Monster{ pPrototype }
{
}

HRESULT CFSM_Mon_DragonTail_Attack::Initialize(FSM_MON_DESC Desc)
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	m_tDesc.pTransform = Desc.pTransform;
	m_tDesc.pNavigation = Desc.pNavigation;
	m_tDesc.pModel = Desc.pModel;
	m_tDesc.tMonDesc = Desc.tMonDesc;

	return S_OK;
}

wstring CFSM_Mon_DragonTail_Attack::Priority_Update(_float fTimeDelta)
{
	//m_tDesc.pTransform->LookAt(m_pPlayerTransform->Get_State(CTransform::STATE_POSITION));

	return TEXT("Monster_DragonTail_State_Attack");
}

wstring CFSM_Mon_DragonTail_Attack::Update(_float fTimeDelta)
{
	if (m_pGameInstance->Key_Down('J'))
	{
		CSkill::SKILL_DESC	BubbleDesc = {};

		BubbleDesc.pOriginTransform = m_tDesc.pTransform;
		BubbleDesc.fSpeedPerSec = 10.f;
		BubbleDesc.fRotationPerSec = XMConvertToRadians(90.0f);
		BubbleDesc.iSkillDamage = 5.f;		
		m_tDesc.pTransform->LookAt(m_pPlayerTransform->Get_State(CTransform::STATE_POSITION));

		for (int i = 1; i <= rand() % 6 + 6;  ++i)
		{
			BubbleDesc.iNumSkill = i;
			m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Skill_Dragon_Bubble"), TEXT("Layer_Skill_Dragon_Bubble"), &BubbleDesc);
		}
	}

	return TEXT("Monster_DragonTail_State_Attack");
}

wstring CFSM_Mon_DragonTail_Attack::Late_Update(_float fTimeDelta)
{
	/*if (m_iAttackType == 0)
	{
		
	}
	else if (m_iAttackType == 1)
	{
		CSkill::SKILL_DESC	BubbleDesc = {};

		BubbleDesc.pOriginTransform = m_tDesc.pTransform;
		BubbleDesc.fSpeedPerSec = 10.f;
		BubbleDesc.fRotationPerSec = XMConvertToRadians(90.0f);
		BubbleDesc.iSkillDamage = 5.f;
		BubbleDesc.iNumSkill = rand() % 6 + 6;

		m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Skill_Dragon_Bubble"), TEXT("Layer_Skill_Dragon_Bubble"), &BubbleDesc);
	}*/
	return TEXT("Monster_DragonTail_State_Attack");
}

void CFSM_Mon_DragonTail_Attack::Enter()
{
	__super::Enter();

	m_strName = TEXT("Monster_DragonTail_State_Attack");

	m_iAttackType = rand() % 2;

	/*if (m_iAttackType == 0)
		m_tDesc.pModel->SetUp_Animation(9, true );
	else if (m_iAttackType == 1)*/
		m_tDesc.pModel->SetUp_Animation(10, true );
	
}

void CFSM_Mon_DragonTail_Attack::Exit()
{
}

CFSM_Mon_DragonTail_Attack* CFSM_Mon_DragonTail_Attack::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, FSM_MON_DESC Desc)
{
	CFSM_Mon_DragonTail_Attack* pInstance = new CFSM_Mon_DragonTail_Attack(pDevice, pContext);

	if (FAILED(pInstance->Initialize(Desc)))
	{
		MSG_BOX(TEXT("Failed to Created : CFSM_Mon_DragonTail_Attack "));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CFSM_Mon_DragonTail_Attack::Free()
{
	__super::Free();
}
