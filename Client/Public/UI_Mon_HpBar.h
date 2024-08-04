#pragma once

#include "UIObject.h"
#include "Monster.h"

BEGIN(Client)

class CUI_Mon_HpBar final :public CUIObject
{
private:
	CUI_Mon_HpBar(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_Mon_HpBar(const CUI_Mon_HpBar& Prototype);
	virtual ~CUI_Mon_HpBar() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta);
	virtual void Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render();

private:
	virtual HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();

private:
	class CMonster* m_pMonster= {};
	_float fHpRatio = {};

public:
	static CUI_Mon_HpBar* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END