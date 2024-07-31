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
	virtual HRESULT Initialize(FSM_MON_DESC Desc);
	virtual wstring Priority_Update(_float fTimeDelta);
	virtual wstring Update(_float fTimeDelta);
	virtual wstring Late_Update(_float fTimeDelta);

public:
	void Create_Bullet();
	

private:
	_uint m_iRanNumAttack = { 0 };
	_uint m_iAttackCnt = { 0 };

	_float m_fAttackTimer = { 0.f };


	_vector m_vPlayerPos = { };

public:
	virtual void Enter() override;
	virtual void Exit()	 override;

public:
	static CFSM_Mon_Archer_Attack* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, FSM_MON_DESC Desc);
	virtual void Free() override;
};

END

