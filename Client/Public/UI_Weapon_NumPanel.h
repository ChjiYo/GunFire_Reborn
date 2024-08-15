#pragma once

#include "UIObject.h"

BEGIN(Client)

class CUI_Weapon_NumPanel final :public CUIObject
{
private:
	CUI_Weapon_NumPanel(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_Weapon_NumPanel(const CUI_Weapon_NumPanel& Prototype);
	virtual ~CUI_Weapon_NumPanel() = default;

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
	static CUI_Weapon_NumPanel* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END