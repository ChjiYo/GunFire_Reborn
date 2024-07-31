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

HRESULT CFSM_Mon_Archer_Idle::Initialize(FSM_MON_DESC tDesc)
{
	m_tDesc.pTransform = tDesc.pTransform;
	m_tDesc.pNavigation = tDesc.pNavigation;
	m_tDesc.pModel = tDesc.pModel;

	return S_OK;
}

wstring CFSM_Mon_Archer_Idle::Priority_Update(_float fTimeDelta)
{	
	return TEXT("Monster_Archer_State_Idle");
}

wstring CFSM_Mon_Archer_Idle::Update(_float fTimeDelta)
{
	return TEXT("Monster_Archer_State_Idle");
}

wstring CFSM_Mon_Archer_Idle::Late_Update(_float fTimeDelta)
{
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(m_pGameInstance->Get_Layer(LEVEL_GAMEPLAY,TEXT("Layer_Player")).front());
	_vector vTargetPos = pPlayer->Get_Transform()->Get_State(CTransform::STATE_POSITION);
	_vector vPos = m_tDesc.pTransform->Get_State(CTransform::STATE_POSITION);

	_vector vDistance = vTargetPos - vPos;
	XMVector4Length(vDistance);
	_float fLength = XMVectorGetX(vDistance);

	if (abs(fLength) < 15.f)
		return TEXT("Monster_Archer_State_Run");

	return TEXT("Monster_Archer_State_Idle");
}

void CFSM_Mon_Archer_Idle::Enter()
{
	__super::Enter();

	m_strName = TEXT("Monster_Archer_State_Idle");
	m_tDesc.pModel->SetUp_Animation(0, true);
}

void CFSM_Mon_Archer_Idle::Exit()
{
}

CFSM_Mon_Archer_Idle* CFSM_Mon_Archer_Idle::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, FSM_MON_DESC tDesc)
{
	CFSM_Mon_Archer_Idle* pInstance = new CFSM_Mon_Archer_Idle(pDevice, pContext);

	if (FAILED(pInstance->Initialize(tDesc)))
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
