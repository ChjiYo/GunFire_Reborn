#pragma once

#include "UIObject.h"

BEGIN(Client)

class CUI_Skill_Panel final :public CUIObject
{
private:
	CUI_Skill_Panel(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_Skill_Panel(const CUI_Skill_Panel& Prototype);
	virtual ~CUI_Skill_Panel() = default;

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
	static CUI_Skill_Panel* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END