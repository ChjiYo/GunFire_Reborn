#include "stdafx.h"

#include "FSM_Mon_DragonTail_In.h"
#include "GameInstance.h"
#include "Animation.h"

#include "Skill.h"
#include "Player.h"

CFSM_Mon_DragonTail_In::CFSM_Mon_DragonTail_In(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CFSM_Monster{ pDevice , pContext }
{
}

CFSM_Mon_DragonTail_In::CFSM_Mon_DragonTail_In(const CFSM_Mon_DragonTail_In& pPrototype)
	:CFSM_Monster{ pPrototype }
{
}

HRESULT CFSM_Mon_DragonTail_In::Initialize(FSM_MON_DESC Desc)
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	m_tDesc.pTransform = Desc.pTransform;
	m_tDesc.pNavigation = Desc.pNavigation;
	m_tDesc.pModel = Desc.pModel;
	m_tDesc.tMonDesc = Desc.tMonDesc;

	return S_OK;
}

wstring CFSM_Mon_DragonTail_In::Priority_Update(_float fTimeDelta)
{
	m_fAnimEndTimer += fTimeDelta;
	m_fAngle++;

	if (m_fAngle >= 360.f)
		m_fAngle = 0.f;

	return TEXT("Monster_DragonTail_State_In");
}

wstring CFSM_Mon_DragonTail_In::Update(_float fTimeDelta)
{
	return TEXT("Monster_DragonTail_State_In");
}

wstring CFSM_Mon_DragonTail_In::Late_Update(_float fTimeDelta)
{
	if (m_fAnimEndTimer >= m_tDesc.pModel->Get_Anim(4)->Get_Duration())
	{
		Warf(10.f, m_fAngle);
		m_fAnimEndTimer = 0.f;

		return TEXT("Monster_DragonTail_State_Out");
	}


	return TEXT("Monster_DragonTail_State_In");
}

void CFSM_Mon_DragonTail_In::Warf(_float fDistance, _float fAngle)
{
	_float WarfPosX = XMVectorGetX(m_tDesc.pTransform->Get_State(CTransform::STATE_POSITION)) + fDistance * cos(fAngle * (3.14f / 180.f));
	_float WarfPosZ = XMVectorGetZ(m_tDesc.pTransform->Get_State(CTransform::STATE_POSITION)) - fDistance * sin(fAngle * (3.14f / 180.f));

	m_tDesc.pTransform->Set_State(CTransform::STATE_POSITION, { WarfPosX  , 0.f, WarfPosZ,1.f});
}

void CFSM_Mon_DragonTail_In::Enter()
{
	__super::Enter();

	m_tDesc.pTransform->LookAt(m_pPlayerTransform->Get_State(CTransform::STATE_POSITION));

	m_tDesc.pModel->SetUp_Animation(4, false);
	m_strName = TEXT("Monster_DragonTail_State_In");
}

void CFSM_Mon_DragonTail_In::Exit()
{
}

CFSM_Mon_DragonTail_In* CFSM_Mon_DragonTail_In::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, FSM_MON_DESC Desc)
{
	CFSM_Mon_DragonTail_In* pInstance = new CFSM_Mon_DragonTail_In(pDevice, pContext);

	if (FAILED(pInstance->Initialize(Desc)))
	{
		MSG_BOX(TEXT("Failed to Created : CFSM_Mon_DragonTail_In"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CFSM_Mon_DragonTail_In::Free()
{
	__super::Free();
}
