#include "stdafx.h"
#include "..\Public\Terrain_Boss.h"

#include "GameInstance.h"

CTerrain_Boss::CTerrain_Boss(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CTerrain{ pDevice, pContext }
{

}

CTerrain_Boss::CTerrain_Boss(const CTerrain_Boss& Prototype)
	: CTerrain{ Prototype }
{

}

HRESULT CTerrain_Boss::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CTerrain_Boss::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	return S_OK;
}

void CTerrain_Boss::Priority_Update(_float fTimeDelta)
{
}

void CTerrain_Boss::Update(_float fTimeDelta)
{
	m_pNavigationCom->Update(m_pTransformCom->Get_WorldMatrix());
}

void CTerrain_Boss::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderObject(CRenderer::RG_NONBLEND, this);


#ifdef _DEBUG
	m_pGameInstance->Add_DebugComponent(m_pNavigationCom);
#endif
}

HRESULT CTerrain_Boss::Render()
{
	if (FAILED(__super::Bind_ShaderResources()))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Begin(0)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Bind_Buffers()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	return S_OK;
}


HRESULT CTerrain_Boss::Ready_Components()
{
	/* Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxNorTex"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Terrain"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom[TEX_DIFFUSE]))))
		return E_FAIL;

	/* Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Terrain"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	/* Com_Navigation */
	if (FAILED(__super::Add_Component(LEVEL_BOSS, TEXT("Prototype_Component_Navigation"),
		TEXT("Com_Navigation"), reinterpret_cast<CComponent**>(&m_pNavigationCom))))
		return E_FAIL;

	return S_OK;
}

CTerrain_Boss* CTerrain_Boss::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CTerrain_Boss* pInstance = new CTerrain_Boss(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CTerrain_Boss"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CTerrain_Boss::Clone(void* pArg)
{
	CTerrain_Boss* pInstance = new CTerrain_Boss(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CTerrain_Boss"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTerrain_Boss::Free()
{

	Safe_Release(m_pNavigationCom);

	for (auto& pTexture : m_pTextureCom)
		Safe_Release(pTexture);

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pVIBufferCom);
	__super::Free();
}
