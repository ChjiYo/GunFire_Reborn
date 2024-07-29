#pragma once

#include "FSM_Monster.h"

BEGIN(Client)

class CFSM_Mon_Archer_Attack final : public CFSM_Monster
{
private:
	CFSM_Mon_Archer_Attack(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CFSM_Mon_Archer_Attack(const CFSM_Mon_Archer_Attack& pPrototype);
	virtual ~CFSM_Mon_Archer_Attack() = default;

public:
	virtual HRESULT Initialize();
	virtual wstring Priority_Update(_float fTimeDelta);
	virtual wstring Update(_float fTimeDelta);
	virtual wstring Late_Update(_float fTimeDelta);

public:
	virtual void Enter() override;
	virtual void Exit()	 override;

public:
	static CFSM_Mon_Archer_Attack* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;
};

END

