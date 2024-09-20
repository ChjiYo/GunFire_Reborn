#pragma once

#include "UIObject.h"
#include "Monster.h"

BEGIN(Engine)

class CTexture;

END

BEGIN(Client)

class CUI_Bullet_Num final :public CUIObject
{
private:
	CUI_Bullet_Num(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_Bullet_Num(const CUI_Bullet_Num& Prototype);
	virtual ~CUI_Bullet_Num() = default;

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
	class CTexture* m_pNumTexture[12] = {};

private:
	_uint m_iTextureIndex = { 0 };
	_uint m_iNum  = { 0 };

public:
	static CUI_Bullet_Num* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END