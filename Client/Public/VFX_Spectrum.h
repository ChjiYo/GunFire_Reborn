#pragma once

#include "Client_Defines.h"
#include "VFX_Monster.h"

BEGIN(Engine)
class CShader;
class CBinModel;
END

BEGIN(Client)

class CVFX_Spectrum final :public CVFX_Monster
{
public:
	enum TEXTURETEX { TEX_DIFFUSE, TEX_MASK, TEX_END };

protected:
	CVFX_Spectrum(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CVFX_Spectrum(const CVFX_Spectrum& Prototype);
	virtual ~CVFX_Spectrum() = default;

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
	CBinModel* m_pModelCom = { nullptr };

private:
	_float m_fTimer = { 0.f };

public:
	static CVFX_Spectrum* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END

