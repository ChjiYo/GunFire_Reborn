#include "stdafx.h"

#include "FSM_Mon_Archer_Attack.h"
#include "GameInstance.h"

#include "Skill.h"
#include "Player.h"

CFSM_Mon_Archer_Attack::CFSM_Mon_Archer_Attack(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CFSM_Monster{ pDevice , pContext }
{
}

CFSM_Mon_Archer_Attack::CFSM_Mon_Archer_Attack(const CFSM_Mon_Archer_Attack& pPrototype)
	:CFSM_Monster{ pPrototype }
{
}

HRESULT CFSM_Mon_Archer_Attack::Initialize(FSM_MON_DESC Desc)
{
	if(FAILED(__super::Initialize()))
		return E_FAIL;

	m_tDesc.pTransform = Desc.pTransform;
	m_tDesc.pNavigation = Desc.pNavigation;
	m_tDesc.pModel = Desc.pModel;

	return S_OK;
}

wstring CFSM_Mon_Archer_Attack::Priority_Update(_float fTimeDelta)
{
	_vector vTargetPos = m_pPlayerTransform->Get_State(CTransform::STATE_POSITION);
	_float4 vFloatPos = {};
	XMStoreFloat4(&vFloatPos, vTargetPos);

	vFloatPos.y = 0.25f;
	m_tDesc.pTransform->LookAt(XMLoadFloat4(&vFloatPos));

	m_fAttackTimer += fTimeDelta;

	return TEXT("Monster_Archer_State_Attack");
}

wstring CFSM_Mon_Archer_Attack::Update(_float fTimeDelta)
{
	if (m_fAttackTimer > 0.25f)
	{
		Create_Bullet();
		m_fAttackTimer = 0.f;
		m_iAttackCnt++;
	}

	return TEXT("Monster_Archer_State_Attack");
}

wstring CFSM_Mon_Archer_Attack::Late_Update(_float fTimeDelta)
{
	if (m_iAttackCnt >= m_iRanNumAttack + 1)
	{
		m_iAttackCnt = 0;
		return TEXT("Monster_Archer_State_Walk");
	}
	return TEXT("Monster_Archer_State_Attack");
}

void CFSM_Mon_Archer_Attack::Create_Bullet()
{
	CSkill::SKILL_DESC	Desc = {};
	
	Desc.pOriginTransform = m_tDesc.pTransform;
	Desc.fSpeedPerSec = 10.f;
	Desc.fRotationPerSec = XMConvertToRadians(90.0f);
	Desc.iSkillDamage = 5.f;
	
	m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Skill_Archer_Bullet"), TEXT("Layer_Skill_Archer_Bullet"), &Desc);
}

void CFSM_Mon_Archer_Attack::Enter()
{
	__super::Enter();

	m_iRanNumAttack = rand() % 4 + 1;

	m_tDesc.pModel->SetUp_Animation( 5, false);
	m_strName = TEXT("Monster_Archer_State_Attack");
}

void CFSM_Mon_Archer_Attack::Exit()
{
}

CFSM_Mon_Archer_Attack* CFSM_Mon_Archer_Attack::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext ,FSM_MON_DESC Desc)
{
	CFSM_Mon_Archer_Attack* pInstance = new CFSM_Mon_Archer_Attack(pDevice, pContext);

	if (FAILED(pInstance->Initialize(Desc)))
	{
		MSG_BOX(TEXT("Failed to Created : CFSM_Mon_Archer_Idle"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CFSM_Mon_Archer_Attack::Free()
{
	__super::Free();
}
