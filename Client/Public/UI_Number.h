#pragma once

#include "UIObject.h"
#include "Client_Defines.h"

BEGIN(Client)

class CUI_Number final :public CUIObject
{
public:
	typedef struct :public  CUIObject::UI_DESC
	{
		_uint iTextureIndex = {};
	}UI_NUM_DESC;
private:
	CUI_Number(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_Number(const CUI_Number& Prototype);
	virtual ~CUI_Number() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta);
	virtual void Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render();

private:
	HRESULT Ready_Component();
	HRESULT Bind_ShaderResources();

private :
	CShader* m_pShaderCom = { nullptr };
	CTexture* m_pTextureCom = { nullptr };
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };

private:
	_uint m_iTextureIndex = { 0 };

public:
	static CUI_Number* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END