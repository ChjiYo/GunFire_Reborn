#pragma once

#include "FSM_Player.h"

BEGIN(Client)

class CFSM_Player_Dash final : public CFSM_Player
{
private:
	CFSM_Player_Dash(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CFSM_Player_Dash(const CFSM_Player_Dash& pPrototype);
	virtual ~CFSM_Player_Dash() = default;

public:
	virtual HRESULT Initialize();
	virtual wstring Priority_Update(_float fTimeDelta);
	virtual wstring Update(_float fTimeDelta);
	virtual wstring Late_Update(_float fTimeDelta);

public:
	virtual void Enter() override;
	virtual void Exit()	 override;

private:
	_vector m_vCurrPos = {};
	_vector m_vPrevPos = {};
	_vector m_vMoveDir = {};
	_vector m_vMovePos = {};

	CTransform* m_pPlayerTransform = { nullptr };

public:
	static CFSM_Player_Dash* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;
};

END

