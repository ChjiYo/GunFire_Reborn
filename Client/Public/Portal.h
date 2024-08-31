#pragma once

#include "Element.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CVIBuffer_Rect;
class CCollider;
END


BEGIN(Client)

class CPortal final : public CElement
{
private:
	CPortal(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CPortal(const CPortal& pPrototype);
	virtual ~CPortal() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta);
	virtual void Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render();

private:
	CShader* m_pShaderCom = { nullptr };
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };
	CTexture* m_pTextureCom = { nullptr };
	CCollider* m_pColliderCom = { nullptr };

private:
	_float m_fTimer = { 0.f };

private:
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();

public:
	static CPortal* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END