#pragma once

#include "FSM_Monster.h"

BEGIN(Client)

class CFSM_Mon_Archer_Walk final : public CFSM_Monster
{
private:
	CFSM_Mon_Archer_Walk(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CFSM_Mon_Archer_Walk(const CFSM_Mon_Archer_Walk& pPrototype);
	virtual ~CFSM_Mon_Archer_Walk() = default;

public:
	virtual HRESULT Initialize();
	virtual void Priority_Update(_float fTimeDelta);
	virtual void Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);

public:
	virtual void Enter() override;
	virtual void Exit()	 override;

public:
	static CFSM_Mon_Archer_Walk* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;
};

END

