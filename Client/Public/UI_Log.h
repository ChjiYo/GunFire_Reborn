#pragma once

#include "UIObject.h"
#include "Monster.h"

BEGIN(Client)

class CUI_Log final :public CUIObject
{
private:
	CUI_Log(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_Log(const CUI_Log& Prototype);
	virtual ~CUI_Log() = default;

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
	wstring m_strWord = {};
	wstring m_strCopyWord = {};
	_float m_fSpeed = { 0.f };
	_uint m_iIndex = { 0 };
	_float m_fDeleteTimer = { 0.f };

public:
	static CUI_Log* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END