#pragma once

#include "UIObject.h"

BEGIN(Client)

class CUI_Cross_Hair final :public CUIObject
{
public:
	typedef struct :public CTransform::TRANSFORM_DESC
	{
		_uint iNum = {};
	}CROSS_HAIR_DESC;

private:
	CUI_Cross_Hair(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_Cross_Hair(const CUI_Cross_Hair& Prototype);
	virtual ~CUI_Cross_Hair() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta);
	virtual void Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render();

private:
	virtual HRESULT Ready_Components();
	void Move(_float fTimeDelta);

private:
	_float m_fMoveTimer = {};
	_bool m_bMoveStop = { true };

	_float  m_fMidPosX = {};
	_float  m_fMidPosY = {};
	_float	m_fAddPos = {};

public:
	static CUI_Cross_Hair* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END

