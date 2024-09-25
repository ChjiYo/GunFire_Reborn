#pragma once

#include "Client_Defines.h"
#include "VFX.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CVIBuffer_Point_Instancing;
END

BEGIN(Client)

class CVFX_In_Dust final :public CVFX
{
protected:
	CVFX_In_Dust(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CVFX_In_Dust(const CVFX_In_Dust& Prototype);
	virtual ~CVFX_In_Dust() = default;

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
	_float m_fScaled = { 1.f };

public:
	static CVFX_In_Dust* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END

