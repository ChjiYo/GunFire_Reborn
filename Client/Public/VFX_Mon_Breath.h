#pragma once

#include "Client_Defines.h"
#include "VFX_Monster.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CVIBuffer_Rect;
END

BEGIN(Client)

class CVFX_Mon_Breath final :public CVFX_Monster
{
public:
	enum TEXTURETEX { TEX_DIFFUSE, TEX_MASK , TEX_END };

protected:
	CVFX_Mon_Breath(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CVFX_Mon_Breath(const CVFX_Mon_Breath& Prototype);
	virtual ~CVFX_Mon_Breath() = default;

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
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };

private:
	_float m_fTimer = {0.f};

public:
	static CVFX_Mon_Breath* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END

