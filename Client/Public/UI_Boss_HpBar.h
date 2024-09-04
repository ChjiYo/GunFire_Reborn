#pragma once

#include "UIObject.h"
#include "Monster.h"

BEGIN(Client)

class CUI_Boss_HpBar final :public CUIObject
{
private:
	CUI_Boss_HpBar(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_Boss_HpBar(const CUI_Boss_HpBar& Prototype);
	virtual ~CUI_Boss_HpBar() = default;

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
	CMonster* m_pBoss= {};

	wstring m_strHp = {};
	wstring m_strMaxHp = {};

	_float m_fHpRatio = { 0.f };

public:
	static CUI_Boss_HpBar* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END