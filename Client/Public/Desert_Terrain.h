#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CVIBuffer_Terrain;
class CNavigation;
END

BEGIN(Client)

class CDesert_Terrain final : public CGameObject
{
public:
	enum TEXTURETEX { TEX_DIFFUSE, TEX_MASK, TEX_BRUSH, TEX_END };

private:
	CDesert_Terrain(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CDesert_Terrain(const CDesert_Terrain& Prototype);
	virtual ~CDesert_Terrain() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	_float3			Get_TerrainPicking() { return m_vPicking; }
	void			Add_NaviMesh(_float3 vPos);

private:
	CShader* m_pShaderCom = { nullptr };
	CTexture* m_pTextureCom[TEX_END] = { nullptr };
	CVIBuffer_Terrain* m_pVIBufferCom = { nullptr };
	CNavigation* m_pNavigationCom = { nullptr };

	_float3						m_vPicking = {};
	_uint						m_iNumNaviPicking = { 0 };
	_float3						vPoints[3] = {};
private:
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();

public:
	static CDesert_Terrain* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END