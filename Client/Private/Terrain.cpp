#include "stdafx.h"
#include "..\Public\Terrain.h"

#include "GameInstance.h"

CTerrain::CTerrain(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CGameObject { pDevice, pContext }
{

}

CTerrain::CTerrain(const CTerrain & Prototype)
	: CGameObject{ Prototype }
{

}

HRESULT CTerrain::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CTerrain::Initialize(void * pArg)
{

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;



	return S_OK;
}

void CTerrain::Priority_Update(_float fTimeDelta)
{
}

void CTerrain::Update(_float fTimeDelta)
{
	m_pNavigationCom->Update(m_pTransformCom->Get_WorldMatrix());
}

void CTerrain::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderObject(CRenderer::RG_NONBLEND, this);


#ifdef _DEBUG
	m_pGameInstance->Add_DebugComponent(m_pNavigationCom);
#endif
}

HRESULT CTerrain::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Begin(0)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Bind_Buffers()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	return S_OK;
}

void CTerrain::Add_NaviMesh(_float3 vPos)
{
	if (vPos.x == 0 && vPos.y == 0 && vPos.z == 0)
		return;

	switch (m_iNumNaviPicking)
	{
	case 0:
		vPoints[m_iNumNaviPicking++] = _float3(vPos.x, vPos.y, vPos.z);
		break;
	case 1:
		vPoints[m_iNumNaviPicking++] = _float3(vPos.x, vPos.y, vPos.z);
		break;
	case 2:
		vPoints[m_iNumNaviPicking++] = _float3(vPos.x, vPos.y, vPos.z);
		break;
	
	default:
		break;
	}

	if (m_iNumNaviPicking == 3)
	{
		m_pNavigationCom->Add_Sell(vPoints);
		m_iNumNaviPicking = 0;
	}

}

HRESULT CTerrain::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;

	if (FAILED(m_pTextureCom[TEX_DIFFUSE]->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture", 0)))
		return E_FAIL;	

	return S_OK;
}

void CTerrain::Free()
{
	__super::Free();
}
