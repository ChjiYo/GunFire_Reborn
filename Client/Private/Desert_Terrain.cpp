#include "stdafx.h"
#include "..\Public\Desert_Terrain.h"

#include "GameInstance.h"

CDesert_Terrain::CDesert_Terrain(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject{ pDevice, pContext }
{

}

CDesert_Terrain::CDesert_Terrain(const CDesert_Terrain& Prototype)
	: CGameObject{ Prototype }
{

}

HRESULT CDesert_Terrain::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CDesert_Terrain::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	return S_OK;
}

void CDesert_Terrain::Priority_Update(_float fTimeDelta)
{
}

void CDesert_Terrain::Update(_float fTimeDelta)
{
	m_pNavigationCom->Update(m_pTransformCom->Get_WorldMatrix());
}

void CDesert_Terrain::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderObject(CRenderer::RG_NONBLEND, this);


#ifdef _DEBUG
	m_pGameInstance->Add_DebugComponent(m_pNavigationCom);
#endif
}

HRESULT CDesert_Terrain::Render()
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

void CDesert_Terrain::Add_NaviMesh(_float3 vPos)
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

HRESULT CDesert_Terrain::Ready_Components()
{
	/* Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_BOSS, TEXT("Prototype_Component_Shader_VtxNorTex"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_BOSS, TEXT("Prototype_Component_Texture_DesertTerrain"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom[TEX_DIFFUSE]))))
		return E_FAIL;

	/* Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_BOSS, TEXT("Prototype_Component_VIBuffer_Terrain"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	/* Com_Navigation */
	if (FAILED(__super::Add_Component(LEVEL_BOSS, TEXT("Prototype_Component_Navigation"),
		TEXT("Com_Navigation"), reinterpret_cast<CComponent**>(&m_pNavigationCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CDesert_Terrain::Bind_ShaderResources()
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

CDesert_Terrain* CDesert_Terrain::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CDesert_Terrain* pInstance = new CDesert_Terrain(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CDesert_Terrain"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CDesert_Terrain::Clone(void* pArg)
{
	CDesert_Terrain* pInstance = new CDesert_Terrain(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CDesert_Terrain"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CDesert_Terrain::Free()
{

	Safe_Release(m_pNavigationCom);

	for (auto& pTexture : m_pTextureCom)
		Safe_Release(pTexture);

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pVIBufferCom);
	__super::Free();
}
