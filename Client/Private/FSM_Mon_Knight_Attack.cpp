#include "stdafx.h"

#include "FSM_Mon_Knight_Attack.h"
#include "GameInstance.h"
#include "Animation.h"

#include "Skill.h"
#include "Player.h"

CFSM_Mon_Knight_Attack::CFSM_Mon_Knight_Attack(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CFSM_Monster{ pDevice , pContext }
{
}

CFSM_Mon_Knight_Attack::CFSM_Mon_Knight_Attack(const CFSM_Mon_Knight_Attack& pPrototype)
	:CFSM_Monster{ pPrototype }
{
}

HRESULT CFSM_Mon_Knight_Attack::Initialize(FSM_MON_DESC Desc)
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	m_tDesc.pTransform = Desc.pTransform;
	m_tDesc.pNavigation = Desc.pNavigation;
	m_tDesc.pModel = Desc.pModel;

	return S_OK;
}

wstring CFSM_Mon_Knight_Attack::Priority_Update(_float fTimeDelta)
{
	m_fAttackEndTime += fTimeDelta;

	_vector vTargetPos = m_pPlayerTransform->Get_State(CTransform::STATE_POSITION);
	_float4 vFloatPos = {};
	XMStoreFloat4(&vFloatPos, vTargetPos);

	vFloatPos.y = 0.25f;
	m_tDesc.pTransform->LookAt(XMLoadFloat4(&vFloatPos));

	return TEXT("Monster_Knight_State_Attack");
}

wstring CFSM_Mon_Knight_Attack::Update(_float fTimeDelta)
{
	return TEXT("Monster_Knight_State_Attack");
}

wstring CFSM_Mon_Knight_Attack::Late_Update(_float fTimeDelta)
{
	_vector vTargetPos = m_pPlayerTransform->Get_State(CTransform::STATE_POSITION);
	_vector vPos = m_tDesc.pTransform->Get_State(CTransform::STATE_POSITION);

	_vector vDistance = vTargetPos - vPos;
	
	_float fLength = XMVectorGetX(XMVector4Length(vDistance));

	if (m_fAttackEndTime >= 0.65f && abs(fLength) < 2.f && m_bAttackEnd)
	{
		CPlayer* pPlayer = dynamic_cast<CPlayer*>(m_pGameInstance->Get_Layer(m_pGameInstance->GetLoadingLevelIndex(), TEXT("Layer_Player")).back());
		pPlayer->Set_Damage(-5.f);
		m_bAttackEnd = false;
	}

	if (m_fAttackEndTime >= m_tDesc.pModel->Get_Anim(13)->Get_Duration())
	{
		m_fAttackEndTime = 0.f;
		m_bAttackEnd = true;

		if (abs(fLength) > 2.f)
		{
			return TEXT("Monster_Knight_State_Run");
		}
	}

	return TEXT("Monster_Knight_State_Attack");
}

void CFSM_Mon_Knight_Attack::Enter()
{
	__super::Enter();

	m_tDesc.pModel->SetUp_Animation(13, true);
	m_strName = TEXT("Monster_Knight_State_Attack");
}

void CFSM_Mon_Knight_Attack::Exit()
{
}

CFSM_Mon_Knight_Attack* CFSM_Mon_Knight_Attack::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, FSM_MON_DESC Desc)
{
	CFSM_Mon_Knight_Attack* pInstance = new CFSM_Mon_Knight_Attack(pDevice, pContext);

	if (FAILED(pInstance->Initialize(Desc)))
	{
		MSG_BOX(TEXT("Failed to Created : CFSM_Mon_Knight_Attack "));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CFSM_Mon_Knight_Attack::Free()
{
	__super::Free();
}
