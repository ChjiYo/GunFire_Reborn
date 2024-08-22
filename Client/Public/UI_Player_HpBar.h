#pragma once

#include "UIObject.h"
#include "Player.h"

BEGIN(Client)

class CUI_Player_HpBar final :public CUIObject
{
private:
	CUI_Player_HpBar(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_Player_HpBar(const CUI_Player_HpBar& Prototype);
	virtual ~CUI_Player_HpBar() = default;

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

	wstring m_strHp = {};
	wstring m_strMaxHp = {};

	_float m_fHpRatio = { 0.f };

public:
	static CUI_Player_HpBar* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END