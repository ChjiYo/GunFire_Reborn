#pragma once

#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CFSM_State abstract : public CBase
{
protected:
	CFSM_State(ID3D11Device* pDevice , ID3D11DeviceContext* pContext);
	CFSM_State(const CFSM_State& Prototype);
	virtual ~CFSM_State() = default;

public:
	virtual HRESULT Initialize();
	virtual void Priority_Update(_float fTimeDelta);
	virtual void Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);

public:
	const wstring& Get_Name() const { return m_strName; }
	void		Set_Name(const wstring& strName) { m_strName = strName; }

public:
	virtual void Enter();
	virtual void Exit();

protected:
	class CState_Machine* m_pStateMachine = { nullptr };
	wstring					m_strName = {};
	class	CGameInstance* m_pGameInstance = {};

public:
	virtual void Free() override;
};

END

