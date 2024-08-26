#pragma once

#include "UIObject.h"
#include "Player.h"

BEGIN(Client)

class CUI_Player_ShieldBar final :public CUIObject
{
private:
	CUI_Player_ShieldBar(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_Player_ShieldBar(const CUI_Player_ShieldBar& Prototype);
	virtual ~CUI_Player_ShieldBar() = default;

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
	CPlayer* m_pPlayer = {};

	wstring m_strShield = { 0 };
	wstring  m_strMaxShield = { 0 };

	_float m_fShieldRatio = { 0.f };

public:
	static CUI_Player_ShieldBar* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END