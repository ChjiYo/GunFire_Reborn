#pragma once

#include "UIObject.h"

BEGIN(Client)

class CUI_Boss_Icon final :public CUIObject
{
private:
	CUI_Boss_Icon(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_Boss_Icon(const CUI_Boss_Icon& Prototype);
	virtual ~CUI_Boss_Icon() = default;

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
	static CUI_Boss_Icon* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END