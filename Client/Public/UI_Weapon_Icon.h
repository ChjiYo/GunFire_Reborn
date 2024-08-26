#pragma once

#include "UIObject.h"
#include "Player.h"

BEGIN(Client)

class CUI_Weapon_Icon final :public CUIObject
{
private:
	CUI_Weapon_Icon(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_Weapon_Icon(const CUI_Weapon_Icon& Prototype);
	virtual ~CUI_Weapon_Icon() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta);
	virtual void Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render();

private:
	virtual HRESULT Ready_Components();

private:
	_uint m_iTextureIcon = { 0 };
	CPlayer::WEAPONID	m_eWeaponID = {};

public:
	static CUI_Weapon_Icon* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END