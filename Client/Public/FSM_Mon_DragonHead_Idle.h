#pragma once

#include "FSM_Monster.h"

BEGIN(Client)

class CFSM_Mon_DragonHead_Idle final : public CFSM_Monster
{
private:
	CFSM_Mon_DragonHead_Idle(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CFSM_Mon_DragonHead_Idle(const CFSM_Mon_DragonHead_Idle& pPrototype);
	virtual ~CFSM_Mon_DragonHead_Idle() = default;

public:
	virtual HRESULT Initialize(FSM_MON_DESC Desc);
	virtual wstring Priority_Update(_float fTimeDelta);
	virtual wstring Update(_float fTimeDelta);
	virtual wstring Late_Update(_float fTimeDelta);

public:
	virtual void Enter() override;
	virtual void Exit()	 override;

private:
	_float m_fAttackDelay = { 0.f };

public:
	static CFSM_Mon_DragonHead_Idle* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, FSM_MON_DESC Desc);
	virtual void Free() override;
};

END

