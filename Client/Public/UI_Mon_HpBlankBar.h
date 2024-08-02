#pragma once

#include "UIObject.h"

BEGIN(Client)

class CUI_Mon_HpBlankBar final :public CUIObject
{
private:
	CUI_Mon_HpBlankBar(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_Mon_HpBlankBar(const CUI_Mon_HpBlankBar& Prototype);
	virtual ~CUI_Mon_HpBlankBar() = default;

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
	static CUI_Mon_HpBlankBar* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END