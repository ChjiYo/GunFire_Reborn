#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Client)

class CItem abstract : public CGameObject
{
protected:
	CItem(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CItem(const CItem& pPrototype);
	virtual ~CItem() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta);
	virtual void Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render();

public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free() override;
};

END