#pragma once

#include "Component.h"
#include "FSM_State.h"

BEGIN(Engine)

class ENGINE_DLL CState_Machine final : public CComponent
{
public:
	typedef struct
	{
		class CTransform* pTransform = { nullptr };
		class CNavigation* pNavigation = { nullptr };
	}STATE_DESC;

private:
	CState_Machine(ID3D11Device* pDevice , ID3D11DeviceContext* pContext);
	CState_Machine(const CState_Machine& Prototype );
	virtual ~CState_Machine() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta);
	virtual void Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);

public:
	CFSM_State* Get_CurState() const { return m_pCurState; }
	HRESULT Set_State(const wstring& strStateTag);
	HRESULT Add_State(CFSM_State* pState, const wstring& strStateTag);

private:
	const _bool Has_State(const wstring& strStateTag);
	CFSM_State* Find_State(const wstring& strStateTag);
	HRESULT Change_State(const wstring& strStateTag);

private:
	map<const wstring, class CFSM_State*> m_pStates;
	CFSM_State* m_pCurState = { nullptr };
	CFSM_State* m_pPrevState = { nullptr };

public:
	static CState_Machine* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END