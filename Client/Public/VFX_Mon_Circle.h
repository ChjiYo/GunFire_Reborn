#pragma once

#include "Client_Defines.h"
#include "VFX_Monster.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CVIBuffer_Point_Instancing;
END

BEGIN(Client)

class CVFX_Mon_Circle final :public CVFX_Monster
{
public:
	enum TEXTURETEX { TEX_DIFFUSE, TEX_MASK, TEX_END };

protected:
	CVFX_Mon_Circle(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CVFX_Mon_Circle(const CVFX_Mon_Circle& Prototype);
	virtual ~CVFX_Mon_Circle() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

protected:
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();
private:
	CShader* m_pShaderCom = { nullptr };
	CTexture* m_pTextureCom = { nullptr };
	CVIBuffer_Point_Instancing* m_pVIBufferCom = { nullptr };

private:
	_float m_fAnimFrame = { 0.f };

public:
	static CVFX_Mon_Circle* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END

