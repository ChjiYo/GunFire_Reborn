#pragma once

#include "Client_Defines.h"
#include "FSM_State.h"

BEGIN(Engine)

class CTransform;
class CNavigation;
class CBinModel;

END

BEGIN(Client)

class  CFSM_Monster abstract : public CFSM_State
{
public:
	typedef struct
	{
		class CTransform* pTransform = { nullptr };
		class CNavigation* pNavigation = { nullptr };
		class CBinModel* pModel = { nullptr };
	}FSM_MON_DESC;

protected:
	CFSM_Monster(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CFSM_Monster(const CFSM_Monster& pPrototype);
	virtual ~CFSM_Monster() = default;

public:
	virtual HRESULT Initialize();
	virtual wstring Priority_Update(_float fTimeDelta);
	virtual wstring Update(_float fTimeDelta);
	virtual wstring Late_Update(_float fTimeDelta);

public:
	virtual void Enter() override;
	virtual void Exit()	 override;

protected:
	FSM_MON_DESC	m_tDesc = {};
	CTransform* m_pTransform = { nullptr };

public:
	virtual void Free() override;
};

END