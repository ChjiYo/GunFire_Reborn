#pragma once

#include "UIObject.h"

BEGIN(Client)

class CUI_Head final :public CUIObject
{
public:
	typedef struct :public CTransform::TRANSFORM_DESC
	{
		_uint iNum = {};
	}CROSS_HAIR_DESC;

private:
	CUI_Head(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_Head(const CUI_Head& Prototype);
	virtual ~CUI_Head() = default;

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
	_float m_fTimer = {};
	_bool m_bMoveStop = { true };

	_float  m_fMidPosX = {};
	_float  m_fMidPosY = {};
	_float	m_fAddPos = {};

	_float2 m_fTotalPos = {  };

public:
	static CUI_Head* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END

