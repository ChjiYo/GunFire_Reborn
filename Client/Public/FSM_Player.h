#pragma once

#include "Client_Defines.h"
#include "FSM_State.h"

BEGIN(Engine)

class CTransform;

END

BEGIN(Client)

class  CFSM_Player abstract : public CFSM_State
{
protected:
	CFSM_Player(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CFSM_Player(const CFSM_Player& pPrototype);
	virtual ~CFSM_Player() = default;

public:
	virtual HRESULT Initialize();
	virtual wstring Priority_Update(_float fTimeDelta);
	virtual wstring Update(_float fTimeDelta);
	virtual wstring Late_Update(_float fTimeDelta);

public:
	virtual void Enter() override;
	virtual void Exit()	 override;

public:
	virtual void Free() override;
};

END