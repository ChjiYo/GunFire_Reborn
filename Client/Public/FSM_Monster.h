#pragma once

#include "Client_Defines.h"
#include "FSM_State.h"

BEGIN(Engine)

class CTransform;

END

BEGIN(Client)

class  CFSM_Monster abstract : public CFSM_State
{
protected:
	CFSM_Monster(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CFSM_Monster(const CFSM_Monster& pPrototype);
	virtual ~CFSM_Monster() = default;

public:
	virtual HRESULT Initialize();
	virtual void Priority_Update(_float fTimeDelta);
	virtual void Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);

public:
	virtual void Enter() override;
	virtual void Exit()	 override;

public:
	virtual void Free() override;
};

END