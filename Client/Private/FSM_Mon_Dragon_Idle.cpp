#include "stdafx.h"

#include "FSM_Mon_Dragon_Idle.h"
#include "GameInstance.h"
#include "Animation.h"

#include "Skill.h"
#include "Player.h"

#include "VFX_Monster.h"
#include "Skill_Monster.h"

CFSM_Mon_Dragon_Idle::CFSM_Mon_Dragon_Idle(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CFSM_Monster{ pDevice , pContext }
{
}

CFSM_Mon_Dragon_Idle::CFSM_Mon_Dragon_Idle(const CFSM_Mon_Dragon_Idle& pPrototype)
	:CFSM_Monster{ pPrototype }
{
}

HRESULT CFSM_Mon_Dragon_Idle::Initialize(FSM_MON_DESC Desc)
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	m_tDesc.pTransform = Desc.pTransform;
	m_tDesc.pNavigation = Desc.pNavigation;
	m_tDesc.pModel = Desc.pModel;
	m_tDesc.tMonDesc = Desc.tMonDesc;
	
	return S_OK;
}

wstring CFSM_Mon_Dragon_Idle::Priority_Update(_float fTimeDelta)
{
	m_fStartAnimTimer += fTimeDelta;

	return TEXT("Monster_Dragon_State_Idle");
}

wstring CFSM_Mon_Dragon_Idle::Update(_float fTimeDelta)
{
	_vector vPlayerPos =  m_pPlayerTransform->Get_State(CTransform::STATE_POSITION);
	_vector vPos = m_tDesc.pTransform->Get_State(CTransform::STATE_POSITION);

	_vector vDir = XMVector3Normalize(XMVectorSetY(vPlayerPos, -3.f) - vPos);

	if (m_bStartAnimEnd)
	{
		_vector vMovePos = vPos + vDir * 5.f * fTimeDelta;
		m_tDesc.pTransform->Set_State(CTransform::STATE_POSITION, vMovePos);
		m_tDesc.pTransform->LookAt(vPlayerPos);

  		_float  vDistance = XMVectorGetX(XMVector3Length(vPlayerPos - vPos));

		if (fabs(vDistance) <= 10.0f)
		{
			m_bNextState = true;
		}
	}



	return TEXT("Monster_Dragon_State_Idle");
}

wstring CFSM_Mon_Dragon_Idle::Late_Update(_float fTimeDelta)
{
	//&& m_tDesc.pModel->Get_Anim(1)->Get_Duration() <= m_fStartAnimTimer
	if (!m_bStartAnimEnd )
	{
		m_fStartAnimTimer = 0.f;
		m_bStartAnimEnd = true;
		m_tDesc.pModel->SetUp_Animation(0, true);
	}

	if (m_bNextState)
	{
		return TEXT("Monster_DragonTail_State_Out");
	}


	return TEXT("Monster_Dragon_State_Idle");
}

void CFSM_Mon_Dragon_Idle::Enter()
{
	__super::Enter();
	
	m_bStartAnimEnd = false;
	m_tDesc.pTransform->LookAt(m_pPlayerTransform->Get_State(CTransform::STATE_POSITION));
	XMVectorSetY(m_tDesc.pTransform->Get_State(CTransform::STATE_POSITION), -3.f);

	CSkill_Monster::SKILL_DESC Desc = {};
	Desc.fSpeedPerSec = 10.f;
	Desc.fRotationPerSec = XMConvertToRadians(90.0f);
	Desc.pOriginTransform = m_tDesc.pTransform;
	Desc.iSkillDamage = 5.f;
	Desc.iNumSkill = 1;
	Desc.iNumParts = CSkill::PART_END;

	m_pGameInstance->Add_GameObject_ToLayer(LEVEL_BOSS, TEXT("Prototype_GameObject_VFX_Dragon_PosCircle"), TEXT("Layer_Skill_Dragon_PosCircle"), &Desc);
		

	m_tDesc.pModel->SetUp_Animation(1, false);
	m_strName = TEXT("Monster_Dragon_State_Idle");
}

void CFSM_Mon_Dragon_Idle::Exit()
{
	m_pGameInstance->Get_Layer(LEVEL_BOSS, TEXT("Layer_Skill_Dragon_PosCircle")).back()->Set_Dead();
}

CFSM_Mon_Dragon_Idle* CFSM_Mon_Dragon_Idle::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, FSM_MON_DESC Desc)
{
	CFSM_Mon_Dragon_Idle* pInstance = new CFSM_Mon_Dragon_Idle(pDevice, pContext);

	if (FAILED(pInstance->Initialize(Desc)))
	{
		MSG_BOX(TEXT("Failed to Created : CFSM_Mon_Dragon_Idle "));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CFSM_Mon_Dragon_Idle::Free()
{
	__super::Free();
}
