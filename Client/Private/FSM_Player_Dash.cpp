#include "stdafx.h"

#include "FSM_Player_Dash.h"

#include "Transform.h"
#include "GameInstance.h"

CFSM_Player_Dash::CFSM_Player_Dash(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CFSM_Player{ pDevice , pContext }
{
}

CFSM_Player_Dash::CFSM_Player_Dash(const CFSM_Player_Dash& pPrototype)
	:CFSM_Player{ pPrototype }
{
}

HRESULT CFSM_Player_Dash::Initialize()
{
	m_pPlayerTransform = dynamic_cast<CTransform*>(m_pGameInstance->Get_Component(LEVEL_STATIC, TEXT("Layer_Player"), TEXT("Com_Transform")));
	Safe_AddRef(m_pPlayerTransform);

	return S_OK;
}

wstring CFSM_Player_Dash::Priority_Update(_float fTimeDelta)
{
	m_vCurrPos = m_pPlayerTransform->Get_State(CTransform::STATE_POSITION);

	return TEXT("");
}

wstring CFSM_Player_Dash::Update(_float fTimeDelta)
{
	m_vPrevPos = m_pPlayerTransform->Get_State(CTransform::STATE_POSITION);

	m_vMoveDir =  m_vPrevPos - m_vCurrPos;

	m_vMoveDir = XMVector3Normalize(m_vMoveDir);

	return TEXT("");
}

wstring CFSM_Player_Dash::Late_Update(_float fTimeDelta)
{
	return TEXT("");
}

void CFSM_Player_Dash::Enter()
{
	m_vMovePos += m_vMoveDir * 5.f;
	m_pPlayerTransform->Set_State(CTransform::STATE_POSITION, m_vMovePos);
}

void CFSM_Player_Dash::Exit()
{

}

CFSM_Player_Dash* CFSM_Player_Dash::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CFSM_Player_Dash* pInstance = new CFSM_Player_Dash(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX(TEXT("Failed to Created : CFSM_Player_Move "));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CFSM_Player_Dash::Free()
{
	Safe_Release(m_pPlayerTransform);

	__super::Free();
}
