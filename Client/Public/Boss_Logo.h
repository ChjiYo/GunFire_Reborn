#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CVIBuffer_Rect;
END

BEGIN(Client)

class CBoss_Logo final : public CGameObject
{
private:
	CBoss_Logo(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CBoss_Logo(const CBoss_Logo& Prototype);
	virtual ~CBoss_Logo() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	CShader* m_pShaderCom = { nullptr };
	CTexture* m_pTextureCom = { nullptr };
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };

private:
	_float					m_fX{}, m_fY{}, m_fSizeX{}, m_fSizeY{};
	_float4x4				m_ViewMatrix{}, m_ProjMatrix{};

private:
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();

public:
	static CBoss_Logo* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END