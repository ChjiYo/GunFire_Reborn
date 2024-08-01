#include "stdafx.h"

#include "FSM_Mon_Archer_Walk.h"
#include "GameInstance.h"

#include "Skill.h"
#include "Player.h"

CFSM_Mon_Archer_Walk::CFSM_Mon_Archer_Walk(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CFSM_Monster{ pDevice , pContext }
{
}

CFSM_Mon_Archer_Walk::CFSM_Mon_Archer_Walk(const CFSM_Mon_Archer_Walk& pPrototype)
	:CFSM_Monster{ pPrototype }
{
}

HRESULT CFSM_Mon_Archer_Walk::Initialize(FSM_MON_DESC tDesc)
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	m_tDesc.pTransform = tDesc.pTransform;
	m_tDesc.pNavigation = tDesc.pNavigation;
	m_tDesc.pModel = tDesc.pModel;

	return S_OK;
}

wstring CFSM_Mon_Archer_Walk::Priority_Update(_float fTimeDelta)
{
	m_fChangeTimer += fTimeDelta;

	return TEXT("Monster_Archer_State_Walk");
}

wstring CFSM_Mon_Archer_Walk::Update(_float fTimeDelta)
{
	_vector vTargetPos = m_pPlayerTransform->Get_State(CTransform::STATE_POSITION);
	_float4 vFloatPos = {};
	XMStoreFloat4(&vFloatPos, vTargetPos);

	vFloatPos.y = 0.25f;
	m_tDesc.pTransform->LookAt(XMLoadFloat4(&vFloatPos));

	Move(fTimeDelta);
	return TEXT("Monster_Archer_State_Walk");
}

wstring CFSM_Mon_Archer_Walk::Late_Update(_float fTimeDelta)
{
	if (m_fChangeTimer > 1.5f)
	{
		m_fChangeTimer = 0.f;
		return TEXT("Monster_Archer_State_Idle");
	}

	return TEXT("Monster_Archer_State_Walk");
}

void CFSM_Mon_Archer_Walk::Enter()
{
	__super::Enter();
	m_iRanDir = rand() % 4;
	Setup_Rendom();
	
	m_strName = TEXT("Monster_Archer_State_Walk");
}

void CFSM_Mon_Archer_Walk::Exit()
{
}

void CFSM_Mon_Archer_Walk::Setup_Rendom()
{
	switch (m_iRanDir)
	{
	case DIR_FRONT:
		m_tDesc.pModel->SetUp_Animation(9, true);
		break;

	case DIR_LEFT:
		m_tDesc.pModel->SetUp_Animation(12, true);
		break;

	case DIR_BACK:
		m_tDesc.pModel->SetUp_Animation(11, true);
		break;

	case DIR_RIGHT:
		m_tDesc.pModel->SetUp_Animation(10, true);
		break;
	}
}

void CFSM_Mon_Archer_Walk::Move(_float fTimeDelta)
{
	switch (m_iRanDir)
	{
	case DIR_FRONT:
		m_tDesc.pTransform->Go_StraightY(fTimeDelta, m_tDesc.pNavigation);
		break;

	case DIR_LEFT:
		m_tDesc.pTransform->Go_Left(fTimeDelta,m_tDesc.pNavigation);
		break;

	case DIR_BACK:
		m_tDesc.pTransform->Go_BackwardY(fTimeDelta, m_tDesc.pNavigation);
		break;

	case DIR_RIGHT:
		m_tDesc.pTransform->Go_Right(fTimeDelta, m_tDesc.pNavigation);
		break;
	}
}

CFSM_Mon_Archer_Walk* CFSM_Mon_Archer_Walk::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, FSM_MON_DESC tDesc)
{
	CFSM_Mon_Archer_Walk* pInstance = new CFSM_Mon_Archer_Walk(pDevice, pContext);

	if (FAILED(pInstance->Initialize(tDesc)))
	{
		MSG_BOX(TEXT("Failed to Created : CFSM_Mon_Archer_Walk "));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CFSM_Mon_Archer_Walk::Free()
{
	__super::Free();
}
