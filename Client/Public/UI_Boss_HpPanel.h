#pragma once

#include "UIObject.h"
#include "Player.h"

BEGIN(Client)

class CUI_Boss_HpPanel final :public CUIObject
{
private:
	CUI_Boss_HpPanel(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_Boss_HpPanel(const CUI_Boss_HpPanel& Prototype);
	virtual ~CUI_Boss_HpPanel() = default;

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
	static CUI_Boss_HpPanel* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END