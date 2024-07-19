#pragma once

#include "FSM_Player.h"

BEGIN(Client)

class CFSM_Player_Jump final : public CFSM_Player
{
private:
	CFSM_Player_Jump(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CFSM_Player_Jump(const CFSM_Player_Jump& pPrototype);
	virtual ~CFSM_Player_Jump() = default;

public:
	virtual HRESULT Initialize();
	virtual void Priority_Update(_float fTimeDelta);
	virtual void Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);

public:
	virtual void Enter() override;
	virtual void Exit()	 override;

public:
	static CFSM_Player_Jump* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;
};

END

