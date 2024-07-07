#pragma once

#include "UIObject.h"

BEGIN(Client)

class CUI_Player_Icon final :public CUIObject
{
private:
	CUI_Player_Icon(ID3D11Device* pDevice , ID3D11DeviceContext* pContext);
	CUI_Player_Icon(const CUI_Player_Icon& Prototype);
	virtual ~CUI_Player_Icon() = default;

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
	static CUI_Player_Icon* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END