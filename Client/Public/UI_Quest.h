#pragma once

#include "UIObject.h"
#include "Monster.h"

BEGIN(Client)

class CUI_Quest final :public CUIObject
{
private:
	CUI_Quest(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_Quest(const CUI_Quest& Prototype);
	virtual ~CUI_Quest() = default;

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
	_float m_fScaleWeight = { 1.f };
	_float m_fPosWeight = { 1.f };

	_bool  m_bFont = { false };

public:
	static CUI_Quest* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END