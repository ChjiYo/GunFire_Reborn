#include "stdafx.h"

#include "FSM_Mon_Spear_Idle.h"
#include "GameInstance.h"

#include "Skill.h"
#include "Player.h"

CFSM_Mon_Spear_Idle::CFSM_Mon_Spear_Idle(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CFSM_Monster{ pDevice , pContext }
{
}

CFSM_Mon_Spear_Idle::CFSM_Mon_Spear_Idle(const CFSM_Mon_Spear_Idle& pPrototype)
	:CFSM_Monster{ pPrototype }
{
}

HRESULT CFSM_Mon_Spear_Idle::Initialize(FSM_MON_DESC Desc)
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	m_tDesc.pTransform = Desc.pTransform;
	m_tDesc.pNavigation = Desc.pNavigation;
	m_tDesc.pModel = Desc.pModel;

	return S_OK;
}

wstring CFSM_Mon_Spear_Idle::Priority_Update(_float fTimeDelta)
{
	m_fAttackDelay += fTimeDelta;

	return TEXT("Monster_Spear_State_Idle");
}

wstring CFSM_Mon_Spear_Idle::Update(_float fTimeDelta)
{
	return TEXT("Monster_Spear_State_Idle");
}

wstring CFSM_Mon_Spear_Idle::Late_Update(_float fTimeDelta)
{
	_vector vTargetPos = m_pPlayerTransform->Get_State(CTransform::STATE_POSITION);
	_vector vPos = m_tDesc.pTransform->Get_State(CTransform::STATE_POSITION);

	_vector vDistance = vTargetPos - vPos;
	
	_float fLength = XMVectorGetX(XMVector4Length(vDistance));

	if (abs(fLength) < 10.f && m_fAttackDelay > 1.5f)
	{
		m_fAttackDelay = 0.f;
		return TEXT("Monster_Spear_State_Run");
	}
	return TEXT("Monster_Spear_State_Idle");
}

void CFSM_Mon_Spear_Idle::Enter()
{
	__super::Enter();

	m_strName = TEXT("Monster_Spear_State_Idle");
	m_tDesc.pModel->SetUp_Animation(2, true);

}

void CFSM_Mon_Spear_Idle::Exit()
{
}

CFSM_Mon_Spear_Idle* CFSM_Mon_Spear_Idle::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, FSM_MON_DESC Desc)
{
	CFSM_Mon_Spear_Idle* pInstance = new CFSM_Mon_Spear_Idle(pDevice, pContext);

	if (FAILED(pInstance->Initialize(Desc)))
	{
		MSG_BOX(TEXT("Failed to Created : CFSM_Mon_Spear_Idle "));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CFSM_Mon_Spear_Idle::Free()
{
	__super::Free();
}
