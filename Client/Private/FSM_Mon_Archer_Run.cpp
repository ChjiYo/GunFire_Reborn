#include "stdafx.h"

#include "FSM_Mon_Archer_Run.h"
#include "GameInstance.h"

#include "Skill.h"
#include "Player.h"

CFSM_Mon_Archer_Run::CFSM_Mon_Archer_Run(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CFSM_Monster{ pDevice , pContext }
{
}

CFSM_Mon_Archer_Run::CFSM_Mon_Archer_Run(const CFSM_Mon_Archer_Run& pPrototype)
	:CFSM_Monster{ pPrototype }
{
}

HRESULT CFSM_Mon_Archer_Run::Initialize(FSM_MON_DESC tDesc)
{
	m_tDesc.pTransform =  tDesc.pTransform;
	m_tDesc.pNavigation = tDesc.pNavigation;
	
	return S_OK;
}

wstring CFSM_Mon_Archer_Run::Priority_Update(_float fTimeDelta)
{
	return TEXT("Monster_Archer_State_Run");
}

wstring CFSM_Mon_Archer_Run::Update(_float fTimeDelta)
{
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(m_pGameInstance->Get_Layer(LEVEL_GAMEPLAY, TEXT("Layer_Player")).front());
	_vector vTargetPos = pPlayer->Get_Transform()->Get_State(CTransform::STATE_POSITION);
	m_tDesc.pTransform->LookAt(vTargetPos);
	

	return TEXT("Monster_Archer_State_Run");
}

wstring CFSM_Mon_Archer_Run::Late_Update(_float fTimeDelta)
{
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(m_pGameInstance->Get_Layer(LEVEL_GAMEPLAY, TEXT("Layer_Player")).front());
	_vector vTargetPos = pPlayer->Get_Transform()->Get_State(CTransform::STATE_POSITION);
	_vector vPos = m_tDesc.pTransform->Get_State(CTransform::STATE_POSITION);

	_vector vDistance = vTargetPos - vPos;
	XMVector4Length(vDistance);
	_float fLength = XMVectorGetX(vDistance);

	if (abs(fLength) > 5.f)
		return TEXT("Monster_Archer_State_Idle");

	return TEXT("Monster_Archer_State_Run");
}

void CFSM_Mon_Archer_Run::Enter()
{
	__super::Enter();

	m_strName = TEXT("Monster_Archer_State_Run");
}

void CFSM_Mon_Archer_Run::Exit()
{
}

CFSM_Mon_Archer_Run* CFSM_Mon_Archer_Run::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, FSM_MON_DESC tDesc)
{
	CFSM_Mon_Archer_Run* pInstance = new CFSM_Mon_Archer_Run(pDevice, pContext);

	if (FAILED(pInstance->Initialize(tDesc)))
	{
		MSG_BOX(TEXT("Failed to Created : CFSM_Mon_Archer_Run "));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CFSM_Mon_Archer_Run::Free()
{
	__super::Free();
}
