#include "stdafx.h"

#include "FSM_Mon_Spear_Attack.h"
#include "GameInstance.h"

#include "Skill.h"
#include "Player.h"

CFSM_Mon_Spear_Attack::CFSM_Mon_Spear_Attack(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CFSM_Monster{ pDevice , pContext }
{
}

CFSM_Mon_Spear_Attack::CFSM_Mon_Spear_Attack(const CFSM_Mon_Spear_Attack& pPrototype)
	:CFSM_Monster{ pPrototype }
{
}

HRESULT CFSM_Mon_Spear_Attack::Initialize(FSM_MON_DESC Desc)
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	m_tDesc.pTransform = Desc.pTransform;
	m_tDesc.pNavigation = Desc.pNavigation;
	m_tDesc.pModel = Desc.pModel;

	return S_OK;
}

wstring CFSM_Mon_Spear_Attack::Priority_Update(_float fTimeDelta)
{
	m_fAttackEndTime += fTimeDelta;

	_vector vTargetPos = m_pPlayerTransform->Get_State(CTransform::STATE_POSITION);
	_float4 vFloatPos = {};
	XMStoreFloat4(&vFloatPos, vTargetPos);

	vFloatPos.y = 0.25f;
	m_tDesc.pTransform->LookAt(XMLoadFloat4(&vFloatPos));

	return TEXT("Monster_Spear_State_Attack");
}

wstring CFSM_Mon_Spear_Attack::Update(_float fTimeDelta)
{
	return TEXT("Monster_Spear_State_Attack");
}

wstring CFSM_Mon_Spear_Attack::Late_Update(_float fTimeDelta)
{
	_vector vTargetPos = m_pPlayerTransform->Get_State(CTransform::STATE_POSITION);
	_vector vPos = m_tDesc.pTransform->Get_State(CTransform::STATE_POSITION);

	_vector vDistance = vTargetPos - vPos;
	XMVector4Length(vDistance);
	_float fLength = XMVectorGetX(vDistance);

	if (m_fAttackEndTime >= 1.4666f)
	{
		m_fAttackEndTime = 0.f;

		if (abs(fLength) > 3.f)
		{
			return TEXT("Monster_Spear_State_Run");
		}
	}

	return TEXT("Monster_Spear_State_Attack");
}

void CFSM_Mon_Spear_Attack::Enter()
{
	__super::Enter();

	m_tDesc.pModel->SetUp_Animation(17, true);
	m_strName = TEXT("Monster_Spear_State_Attack");
}

void CFSM_Mon_Spear_Attack::Exit()
{
}

CFSM_Mon_Spear_Attack* CFSM_Mon_Spear_Attack::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, FSM_MON_DESC Desc)
{
	CFSM_Mon_Spear_Attack* pInstance = new CFSM_Mon_Spear_Attack(pDevice, pContext);

	if (FAILED(pInstance->Initialize(Desc)))
	{
		MSG_BOX(TEXT("Failed to Created : CFSM_Mon_Spear_Attack "));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CFSM_Mon_Spear_Attack::Free()
{
	__super::Free();
}
 