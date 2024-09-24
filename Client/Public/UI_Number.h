#pragma once

#include "UIObject.h"
#include "Client_Defines.h"

BEGIN(Engine)

class CTransform;

END

BEGIN(Client)

class CUI_Number final :public CUIObject
{
public:
	typedef struct :public  CUIObject::UI_DESC
	{
		_bool bHead = { false };
		_uint iTextureIndex = {0};
		_uint iNumCnt = { 0 };
		_uint iAngle = { 0 };
		_uint iStatic = { 0 };
		
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

private:
	void Destroy(_float fTimeDelta);

private:
	class CTransform* m_pOriginTransform = { nullptr };

private:
	_bool m_bHead = { false };

	_uint m_iTextureIndex = { 0 };
	_uint m_iNumTexture = { 0 };
	_uint m_iAngle = { 0 };
	_uint m_iNiumCnt = { 0 };

	_float m_fDeleteTimer = { 0.f };

private:
	_vector m_vFirstPos = {};
	_vector m_vFirstRight = {};

public:
	static CUI_Number* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END