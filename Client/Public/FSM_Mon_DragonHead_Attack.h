#pragma once

#include "FSM_Monster.h"

BEGIN(Client)

class CFSM_Mon_DragonHead_Attack final : public CFSM_Monster
{
private:
	CFSM_Mon_DragonHead_Attack(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CFSM_Mon_DragonHead_Attack(const CFSM_Mon_DragonHead_Attack& pPrototype);
	virtual ~CFSM_Mon_DragonHead_Attack() = default;

public:
	virtual HRESULT Initialize(FSM_MON_DESC Desc);
	virtual wstring Priority_Update(_float fTimeDelta);
	virtual wstring Update(_float fTimeDelta);
	virtual wstring Late_Update(_float fTimeDelta);

public:
	virtual void Enter() override;
	virtual void Exit()	 override;

private:
	_bool	m_bSeccondAttack = { false };
	_bool	m_bAttackEnd = { false };

	_float m_fAttackEndTimer = { 0.f };
	_float  m_fSkillCreateTimer = { 0.f };

public:
	static CFSM_Mon_DragonHead_Attack* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, FSM_MON_DESC Desc);
	virtual void Free() override;
};

END

