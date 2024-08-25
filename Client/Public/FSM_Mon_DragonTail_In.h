#pragma once

#include "FSM_Monster.h"

BEGIN(Client)

class CFSM_Mon_DragonTail_In final : public CFSM_Monster
{
private:
	CFSM_Mon_DragonTail_In(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CFSM_Mon_DragonTail_In(const CFSM_Mon_DragonTail_In& pPrototype);
	virtual ~CFSM_Mon_DragonTail_In() = default;

public:
	virtual HRESULT Initialize(FSM_MON_DESC Desc);
	virtual wstring Priority_Update(_float fTimeDelta);
	virtual wstring Update(_float fTimeDelta);
	virtual wstring Late_Update(_float fTimeDelta);

private:
	void Warf(_float fDistance, _float fAngle);

public:
	virtual void Enter() override;
	virtual void Exit()	 override;

private:
	_float m_fAnimEndTimer = { 0.f };
	_float m_fAngle = { 0.f };
public:
	static CFSM_Mon_DragonTail_In* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, FSM_MON_DESC Desc);
	virtual void Free() override;
};

END

