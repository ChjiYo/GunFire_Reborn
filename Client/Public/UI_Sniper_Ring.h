#pragma once

#include "UIObject.h"

BEGIN(Client)

class CUI_Sniper_Ring final :public CUIObject
{

private:
	CUI_Sniper_Ring(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_Sniper_Ring(const CUI_Sniper_Ring& Prototype);
	virtual ~CUI_Sniper_Ring() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta);
	virtual void Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render();

private:
	virtual HRESULT Ready_Components();
	 

public:
	static CUI_Sniper_Ring* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END