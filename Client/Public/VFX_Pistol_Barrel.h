#pragma once

#include "VFX.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CVIBuffer_Rect;
class CCollider;
END

class CVFX_Pistol_Barrel final :public CVFX
{
protected:
	CVFX_Pistol_Barrel(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CVFX_Pistol_Barrel(const CVFX_Pistol_Barrel& Prototype);
	virtual ~CVFX_Pistol_Barrel() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();

private:
	void Destroy(_float fTimeDelta);

private:
	CShader* m_pShaderCom = { nullptr };
	CTexture* m_pTextureCom = { nullptr };
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };

private:
	_float m_fAnimFream = { 0.f };

public:
	static CVFX_Pistol_Barrel* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

