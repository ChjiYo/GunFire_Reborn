#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

#include "GameInstance.h"

BEGIN(Engine)

class CVIBuffer_Rect;
class CTexture; 
class CShader;

END

BEGIN(Client)

class CUIObject abstract : public CGameObject
{
public:
	typedef struct
	{
		class CTransform* pTransform = {};
		CGameObject* pGameObject = {};
	}UI_DESC;

protected:
	CUIObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUIObject(const CUIObject& Prototype);
	virtual ~CUIObject() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta);
	virtual void Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render();

protected:
	virtual HRESULT Bind_ShaderResources();
	virtual HRESULT Bind_Perspective();

	virtual HRESULT Ready_Components();

	virtual void Set_UI_Setting(_float fSizeX, _float fSizeY , _float fPosX , _float fPosY);

protected:
	CShader* m_pShaderCom = { nullptr };
	CTexture* m_pTextureCom = { nullptr };
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };

protected:
	_float4x4 m_ViewMatrix = {};
	_float4x4 m_ProjMatrix = {}; // Projection 행렬 멤버 변수 추가

	_float m_fSizeX = { 0.f };
	_float m_fSizeY = { 0.f };
	_float m_fPosX = { 0.f };
	_float m_fPosY = { 0.f };

	CTransform* m_pTargetTransform = {};
public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free() override;
};

END

