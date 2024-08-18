#pragma once

#include "FSM_Monster.h"

BEGIN(Client)

class CFSM_Mon_Boom_Idle final : public CFSM_Monster
{
private:
	CFSM_Mon_Boom_Idle(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CFSM_Mon_Boom_Idle(const CFSM_Mon_Boom_Idle& pPrototype);
	virtual ~CFSM_Mon_Boom_Idle() = default;

public:
	virtual HRESULT Initialize(FSM_MON_DESC Desc);
	virtual wstring Priority_Update(_float fTimeDelta);
	virtual wstring Update(_float fTimeDelta);
	virtual wstring Late_Update(_float fTimeDelta);

public:
	virtual void Enter() override;
	virtual void Exit()	 override;

public:
	static CFSM_Mon_Boom_Idle* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, FSM_MON_DESC Desc);
	virtual void Free() override;
};

END

