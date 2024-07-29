#pragma once

#include "FSM_Player.h"

BEGIN(Client)

class CFSM_Player_Attack final : public CFSM_Player
{
private:
	CFSM_Player_Attack(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CFSM_Player_Attack(const CFSM_Player_Attack& pPrototype);
	virtual ~CFSM_Player_Attack() = default;

public:
	virtual HRESULT Initialize();
	virtual wstring Priority_Update(_float fTimeDelta);
	virtual wstring Update(_float fTimeDelta);
	virtual wstring Late_Update(_float fTimeDelta);

public:
	virtual void Enter() override;
	virtual void Exit()	 override;

public:
	static CFSM_Player_Attack* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;
};

END

