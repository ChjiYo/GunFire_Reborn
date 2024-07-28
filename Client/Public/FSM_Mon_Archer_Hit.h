#pragma once

#include "FSM_Player.h"

BEGIN(Client)

class CFSM_Mon_Archer_Hit final : public CFSM_Player
{
private:
	CFSM_Mon_Archer_Hit(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CFSM_Mon_Archer_Hit(const CFSM_Mon_Archer_Hit& pPrototype);
	virtual ~CFSM_Mon_Archer_Hit() = default;

public:
	virtual HRESULT Initialize();
	virtual void Priority_Update(_float fTimeDelta);
	virtual void Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);

public:
	virtual void Enter() override;
	virtual void Exit()	 override;

public:
	static CFSM_Mon_Archer_Hit* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;
};

END

