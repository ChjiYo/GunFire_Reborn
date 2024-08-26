#include "stdafx.h"

#include "FSM_Mon_Boom_Idle.h"
#include "GameInstance.h"

#include "Skill.h"
#include "Player.h"

CFSM_Mon_Boom_Idle::CFSM_Mon_Boom_Idle(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CFSM_Monster{ pDevice , pContext }
{
}

CFSM_Mon_Boom_Idle::CFSM_Mon_Boom_Idle(const CFSM_Mon_Boom_Idle& pPrototype)
	:CFSM_Monster{ pPrototype }
{
}

HRESULT CFSM_Mon_Boom_Idle::Initialize(FSM_MON_DESC Desc)
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	m_tDesc.pTransform = Desc.pTransform;
	m_tDesc.pNavigation = Desc.pNavigation;
	m_tDesc.pModel = Desc.pModel;

	return S_OK;
}

wstring CFSM_Mon_Boom_Idle::Priority_Update(_float fTimeDelta)
{
	return TEXT("Monster_Boom_State_Idle");
}

wstring CFSM_Mon_Boom_Idle::Update(_float fTimeDelta)
{
	return TEXT("Monster_Boom_State_Idle");
}

wstring CFSM_Mon_Boom_Idle::Late_Update(_float fTimeDelta)
{
	_vector vTargetPos = m_pPlayerTransform->Get_State(CTransform::STATE_POSITION);
	_vector vPos = m_tDesc.pTransform->Get_State(CTransform::STATE_POSITION);

	_vector vDistance = vTargetPos - vPos;
	XMVector4Length(vDistance);
	_float fLength = XMVectorGetX(vDistance);

	if (abs(fLength) < 10.f)
	{
		return TEXT("Monster_Boom_State_Run");
	}

	return TEXT("Monster_Boom_State_Idle");
}

void CFSM_Mon_Boom_Idle::Enter()
{
	__super::Enter();

	m_strName = TEXT("Monster_Boom_State_Idle");
}

void CFSM_Mon_Boom_Idle::Exit()
{
}

CFSM_Mon_Boom_Idle* CFSM_Mon_Boom_Idle::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, FSM_MON_DESC Desc)
{
	CFSM_Mon_Boom_Idle* pInstance = new CFSM_Mon_Boom_Idle(pDevice, pContext);

	if (FAILED(pInstance->Initialize(Desc)))
	{
		MSG_BOX(TEXT("Failed to Created : CFSM_Mon_Boom_Idle "));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CFSM_Mon_Boom_Idle::Free()
{
	__super::Free();
}
