#pragma once

#include "FSM_Monster.h"

BEGIN(Client)

class CFSM_Mon_DragonHead_Out final : public CFSM_Monster
{
private:
	CFSM_Mon_DragonHead_Out(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CFSM_Mon_DragonHead_Out(const CFSM_Mon_DragonHead_Out& pPrototype);
	virtual ~CFSM_Mon_DragonHead_Out() = default;

public:
	virtual HRESULT Initialize(FSM_MON_DESC Desc);
	virtual wstring Priority_Update(_float fTimeDelta);
	virtual wstring Update(_float fTimeDelta);
	virtual wstring Late_Update(_float fTimeDelta);

public:
	virtual void Enter() override;
	virtual void Exit()	 override;

private:
	_float m_fNextAnimFrame = {0.f};
	_float m_fStartTimer = { 0.f };

	_bool m_bStart = { false };
	_bool m_bCheck = { false };

	_float AnimTimer1 = { 0.f };

	_bool m_bStraightEnd = { false };
	_bool m_bLast = { false };

public:
	static CFSM_Mon_DragonHead_Out* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, FSM_MON_DESC Desc);
	virtual void Free() override;
};

END

