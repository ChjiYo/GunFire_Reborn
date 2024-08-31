#include "stdafx.h"

#include "FSM_Mon_Bug_Attack.h"
#include "GameInstance.h"
#include "Animation.h"

#include "Skill.h"
#include "Player.h"

CFSM_Mon_Bug_Attack::CFSM_Mon_Bug_Attack(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CFSM_Monster{ pDevice , pContext }
{
}

CFSM_Mon_Bug_Attack::CFSM_Mon_Bug_Attack(const CFSM_Mon_Bug_Attack& pPrototype)
	:CFSM_Monster{ pPrototype }
{
}

HRESULT CFSM_Mon_Bug_Attack::Initialize(FSM_MON_DESC Desc)
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	m_tDesc.pTransform = Desc.pTransform;
	m_tDesc.pNavigation = Desc.pNavigation;
	m_tDesc.pModel = Desc.pModel;

	return S_OK;
}

wstring CFSM_Mon_Bug_Attack::Priority_Update(_float fTimeDelta)
{
	m_fAttackTime += fTimeDelta;
	m_fDamageDelay += fTimeDelta;

	return TEXT("Monster_Bug_State_Attack");
}

wstring CFSM_Mon_Bug_Attack::Update(_float fTimeDelta)
{
	_vector vTargetPos = m_pPlayerTransform->Get_State(CTransform::STATE_POSITION);
	_vector vPos = m_tDesc.pTransform->Get_State(CTransform::STATE_POSITION);

	_vector vDistance = vTargetPos - vPos;
	XMVector4Length(vDistance);
	_float fLength = XMVectorGetX(vDistance);

	CCollider* pTargetCollider = dynamic_cast<CCollider*>(m_pGameInstance->Get_Component(LEVEL_STATIC, TEXT("Layer_Player"), TEXT("Com_Collider"), 0));
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(m_pGameInstance->Get_Layer(LEVEL_STATIC, TEXT("Layer_Player")).front());

	if (m_fDamageDelay >= m_tDesc.pModel->Get_Anim(4)->Get_Duration() - 0.25f && abs(fLength) <= 2.f)
	{
		pPlayer->Set_Damage(-10);
		m_fDamageDelay = 0.f;
	}

	return TEXT("Monster_Bug_State_Attack");
}

wstring CFSM_Mon_Bug_Attack::Late_Update(_float fTimeDelta)
{
	_vector vTargetPos = m_pPlayerTransform->Get_State(CTransform::STATE_POSITION);
	_vector vPos = m_tDesc.pTransform->Get_State(CTransform::STATE_POSITION);

	_vector vDistance = vTargetPos - vPos;
	XMVector4Length(vDistance);
	_float fLength = XMVectorGetX(vDistance);

	if (m_fAttackTime >= m_tDesc.pModel->Get_Anim(4)->Get_Duration())
	{
		m_fAttackTime = 0.f;

		if (abs(fLength) > 3.f)
		{
			return TEXT("Monster_Bug_State_Run");
		}
		else
			return TEXT("Monster_Bug_State_Idle");
	}

	return TEXT("Monster_Bug_State_Attack");
}

void CFSM_Mon_Bug_Attack::Enter()
{
	__super::Enter();

	m_tDesc.pModel->SetUp_Animation(4,false);
	m_fDamageDelay = 0.f;
	m_strName = TEXT("Monster_Bug_State_Attack");

}

void CFSM_Mon_Bug_Attack::Exit()
{
}

CFSM_Mon_Bug_Attack* CFSM_Mon_Bug_Attack::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, FSM_MON_DESC Desc)
{
	CFSM_Mon_Bug_Attack* pInstance = new CFSM_Mon_Bug_Attack(pDevice, pContext);

	if (FAILED(pInstance->Initialize(Desc)))
	{
		MSG_BOX(TEXT("Failed to Created : CFSM_Mon_Bug_Attack "));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CFSM_Mon_Bug_Attack::Free()
{
	__super::Free();
}
