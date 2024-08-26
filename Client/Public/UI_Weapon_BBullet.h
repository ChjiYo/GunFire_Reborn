#pragma once

#include "UIObject.h"

BEGIN(Client)

class CUI_Weapon_BBullet final :public CUIObject
{
private:
	CUI_Weapon_BBullet(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_Weapon_BBullet(const CUI_Weapon_BBullet& Prototype);
	virtual ~CUI_Weapon_BBullet() = default;

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
	static CUI_Weapon_BBullet* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END