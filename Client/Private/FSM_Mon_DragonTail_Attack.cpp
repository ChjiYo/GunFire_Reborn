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
	m_fAttackDelay += fTimeDelta;
	m_fNextStateTimer += fTimeDelta;

	if (m_iAttackType % 2 == 0)
	{
		if (m_tDesc.pModel->Get_Anim(10)->Get_Duration() <= m_fAttackDelay )
		{
			m_bAttackEnable = false;
			m_fAttackDelay = 0.f;
		}
	}
	else
	{
		if (m_tDesc.pModel->Get_Anim(9)->Get_Duration() <= m_fAttackDelay)
		{
			m_bAttackEnable = false;
			m_fAttackDelay = 0.f;
		}
	}


	return TEXT("Monster_DragonTail_State_Attack");
}

wstring CFSM_Mon_DragonTail_Attack::Update(_float fTimeDelta)
{
	if (m_iAttackType % 2 == 0)
	{
		if (m_tDesc.pModel->Get_Anim(10)->Get_Duration() -0.5f <= m_fAttackDelay && !m_bAttackEnable)
		{
			m_bAttackEnable = true;
			CSkill::SKILL_DESC	BubbleDesc = {};

			BubbleDesc.pOriginTransform = m_tDesc.pTransform;
			BubbleDesc.fSpeedPerSec = 10.f;
			BubbleDesc.fRotationPerSec = XMConvertToRadians(90.0f);
			BubbleDesc.iSkillDamage = 5.f;
			m_tDesc.pTransform->LookAt(m_pPlayerTransform->Get_State(CTransform::STATE_POSITION));

			for (int i = 1; i <= rand() % 6 + 6; ++i)
			{
				BubbleDesc.iNumSkill = i;
				m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Skill_Dragon_Bubble"), TEXT("Layer_Skill_Dragon_Bubble"), &BubbleDesc);
			}
			m_iAttackCnt++;
		}
	}
	else 
	{
		if (m_tDesc.pModel->Get_Anim(9)->Get_Duration()- 0.5f <= m_fAttackDelay && !m_bAttackEnable)
		{
			m_bAttackEnable = true;
			CSkill::SKILL_DESC	DustDesc = {};

			DustDesc.pOriginTransform = m_tDesc.pTransform;
			DustDesc.fSpeedPerSec = 10.f;
			DustDesc.fRotationPerSec = XMConvertToRadians(90.0f);
			DustDesc.iSkillDamage = 5.f;
			DustDesc.iNumParts = CSkill::PART_END;
			m_tDesc.pTransform->LookAt(m_pPlayerTransform->Get_State(CTransform::STATE_POSITION));

			m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Skill_Dragon_Dust"), TEXT("Layer_Skill_Dragon_Dust"), &DustDesc);

			m_iAttackCnt++;
		}
	}

	return TEXT("Monster_DragonTail_State_Attack");
}

wstring CFSM_Mon_DragonTail_Attack::Late_Update(_float fTimeDelta)
{
	//if (m_iAttackCnt >= 2)
	//{
	//	m_iAttackCnt = 0;
	//	return TEXT("Monster_DragonTail_State_In");
	//}

	if (m_iAttackType % 2 == 0)
	{
		if (m_tDesc.pModel->Get_Anim(10)->Get_Duration() * 2 <= m_fNextStateTimer)
		{
			m_fNextStateTimer = 0.f;
			return TEXT("Monster_DragonTail_State_In");
		}
	}
	else
	{
		if (m_tDesc.pModel->Get_Anim(9)->Get_Duration() * 2 <= m_fNextStateTimer)
		{
			m_fNextStateTimer = 0.f;
			return TEXT("Monster_DragonTail_State_In");
		}
	}

	return TEXT("Monster_DragonTail_State_Attack");
}

void CFSM_Mon_DragonTail_Attack::Enter()
{
	__super::Enter();

	m_strName = TEXT("Monster_DragonTail_State_Attack");

	m_tDesc.pTransform->LookAt(m_pPlayerTransform->Get_State(CTransform::STATE_POSITION));

	m_iAttackType++;

		//rand() % 2;
	_vector vUpdatePos = XMVectorSetY(m_tDesc.pTransform->Get_State(CTransform::STATE_POSITION), -0.5f);
	m_tDesc.pTransform->Set_State(CTransform::STATE_POSITION, vUpdatePos);

	if (m_iAttackType % 2 == 0)
	m_tDesc.pModel->SetUp_Animation(10, true);

	else 
		m_tDesc.pModel->SetUp_Animation(9, true );
	
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
