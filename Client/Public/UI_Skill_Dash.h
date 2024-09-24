#pragma once

#include "UIObject.h"

BEGIN(Client)

class CUI_Skill_Dash final :public CUIObject
{
private:
	CUI_Skill_Dash(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_Skill_Dash(const CUI_Skill_Dash& Prototype);
	virtual ~CUI_Skill_Dash() = default;

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
	_bool m_bDash = { false };
	_float  m_bDashTimer = { 0.f};

public:
	static CUI_Skill_Dash* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END