#include "stdafx.h"

#include "FSM_Mon_Boom_Run.h"
#include "GameInstance.h"

#include "Skill.h"
#include "Player.h"

CFSM_Mon_Boom_Run::CFSM_Mon_Boom_Run(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CFSM_Monster{ pDevice , pContext }
{
}

CFSM_Mon_Boom_Run::CFSM_Mon_Boom_Run(const CFSM_Mon_Boom_Run& pPrototype)
	:CFSM_Monster{ pPrototype }
{
}

HRESULT CFSM_Mon_Boom_Run::Initialize(FSM_MON_DESC Desc)
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	m_tDesc.pTransform = Desc.pTransform;
	m_tDesc.pNavigation = Desc.pNavigation;
	m_tDesc.pModel = Desc.pModel;

	return S_OK;
}

wstring CFSM_Mon_Boom_Run::Priority_Update(_float fTimeDelta)
{
	return TEXT("Monster_Boom_State_Run");
}

wstring CFSM_Mon_Boom_Run::Update(_float fTimeDelta)
{
	_vector vTargetPos = m_pPlayerTransform->Get_State(CTransform::STATE_POSITION);
	_float4 vFloatPos = {};
	XMStoreFloat4(&vFloatPos, vTargetPos);

	vFloatPos.y = 0.25f;
	m_tDesc.pTransform->LookAt(XMLoadFloat4(&vFloatPos));
	m_tDesc.pTransform->Go_StraightY(fTimeDelta, m_tDesc.pNavigation);

	return TEXT("Monster_Boom_State_Run");
}

wstring CFSM_Mon_Boom_Run::Late_Update(_float fTimeDelta)
{
	_vector vTargetPos = m_pPlayerTransform->Get_State(CTransform::STATE_POSITION);
	_vector vPos = m_tDesc.pTransform->Get_State(CTransform::STATE_POSITION);

	_vector vDistance = vTargetPos - vPos;
	
	_float fLength = XMVectorGetX(XMVector4Length(vDistance));

	if (abs(fLength) < 1.5f)
		return TEXT("Monster_Boom_State_Attack");

	return TEXT("Monster_Boom_State_Run");
}

void CFSM_Mon_Boom_Run::Enter()
{
	__super::Enter();

	m_strName = TEXT("Monster_Boom_State_Run");
	m_tDesc.pModel->SetUp_Animation(5, true);
}

void CFSM_Mon_Boom_Run::Exit()
{
}

CFSM_Mon_Boom_Run* CFSM_Mon_Boom_Run::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, FSM_MON_DESC Desc)
{
	CFSM_Mon_Boom_Run* pInstance = new CFSM_Mon_Boom_Run(pDevice, pContext);

	if (FAILED(pInstance->Initialize(Desc)))
	{
		MSG_BOX(TEXT("Failed to Created : CFSM_Mon_Boom_Run "));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CFSM_Mon_Boom_Run::Free()
{
	__super::Free();
}
