#include "stdafx.h"

#include "FSM_Mon_DragonHead_Out.h"
#include "GameInstance.h"
#include "Animation.h"

#include "Skill.h"
#include "Player.h"

CFSM_Mon_DragonHead_Out::CFSM_Mon_DragonHead_Out(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CFSM_Monster{ pDevice , pContext }
{
}

CFSM_Mon_DragonHead_Out::CFSM_Mon_DragonHead_Out(const CFSM_Mon_DragonHead_Out& pPrototype)
	:CFSM_Monster{ pPrototype }
{
}

HRESULT CFSM_Mon_DragonHead_Out::Initialize(FSM_MON_DESC Desc)
{
	if (FAILED(__super::Initialize()))
		return E_FAIL;

	m_tDesc.pTransform = Desc.pTransform;
	m_tDesc.pNavigation = Desc.pNavigation;
	m_tDesc.pModel = Desc.pModel;
	m_tDesc.tMonDesc = Desc.tMonDesc;

	return S_OK;
}

wstring CFSM_Mon_DragonHead_Out::Priority_Update(_float fTimeDelta)
{
	m_fHitMotionTime += fTimeDelta;

	return TEXT("Monster_Knight_State_Hit");
}

wstring CFSM_Mon_DragonHead_Out::Update(_float fTimeDelta)
{
	return TEXT("Monster_Knight_State_Hit");
}

wstring CFSM_Mon_DragonHead_Out::Late_Update(_float fTimeDelta)
{
	if (m_fHitMotionTime >= m_tDesc.pModel->Get_Anim(4)->Get_Duration())
	{
		return TEXT("Monster_Knight_State_Run");
	}
	return TEXT("Monster_Knight_State_Hit");
}

void CFSM_Mon_DragonHead_Out::Enter()
{
	__super::Enter();

	m_tDesc.pTransform->LookAt(m_pPlayerTransform->Get_State(CTransform::STATE_POSITION));

	m_tDesc.pModel->SetUp_Animation(4, false);
	m_strName = TEXT("Monster_Knight_State_Hit");
}

void CFSM_Mon_DragonHead_Out::Exit()
{
}

CFSM_Mon_DragonHead_Out* CFSM_Mon_DragonHead_Out::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, FSM_MON_DESC Desc)
{
	CFSM_Mon_DragonHead_Out* pInstance = new CFSM_Mon_DragonHead_Out(pDevice, pContext);

	if (FAILED(pInstance->Initialize(Desc)))
	{
		MSG_BOX(TEXT("Failed to Created : CFSM_Mon_DragonHead_Out "));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CFSM_Mon_DragonHead_Out::Free()
{
	__super::Free();
}
