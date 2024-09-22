#include "stdafx.h"
#include "Terrain_GamePlay.h"

#include "GameInstance.h"

CTerrain_GamePlay::CTerrain_GamePlay(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CTerrain{ pDevice, pContext }
{

}

CTerrain_GamePlay::CTerrain_GamePlay(const CTerrain_GamePlay& Prototype)
	: CTerrain{ Prototype }
{

}

HRESULT CTerrain_GamePlay::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CTerrain_GamePlay::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	return S_OK;
}

void CTerrain_GamePlay::Priority_Update(_float fTimeDelta)
{
}

void CTerrain_GamePlay::Update(_float fTimeDelta)
{
	m_pNavigationCom->Update(m_pTransformCom->Get_WorldMatrix());
	//m_pColliderCom->Update(m_pTransformCom->Get_WorldMatrix());
}

void CTerrain_GamePlay::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderObject(CRenderer::RG_NONBLEND, this);


#ifdef _DEBUG
	m_pGameInstance->Add_DebugComponent(m_pNavigationCom);
	//m_pGameInstance->Add_DebugComponent(m_pColliderCom);

#endif
}

HRESULT CTerrain_GamePlay::Render()
{
	if (FAILED(__super::Bind_ShaderResources()))
		return E_FAIL;

	if (FAILED(m_pTextureCom[TEX_DIFFUSE]->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture", 1)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Begin(0)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Bind_Buffers()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	return S_OK;
}


HRESULT CTerrain_GamePlay::Ready_Components()
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
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Navigation"),
		TEXT("Com_Navigation"), reinterpret_cast<CComponent**>(&m_pNavigationCom))))
		return E_FAIL;

	/* For.Com_Collider */
	/*CBounding_OBB::BOUNDING_OBB_DESC	BoundingDesc{};

	BoundingDesc.vExtents = _float3(0.5f, 0.5f, 0.5f);
	BoundingDesc.vCenter = _float3(0.f, BoundingDesc.vExtents.y * 0.5f , 0.f);
	BoundingDesc.vRotation = _float3(0.f, 0, 0.f);
	BoundingDesc.pObject = this;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_OBB"),
		TEXT("Com_ColliderOBB"), reinterpret_cast<CComponent**>(&m_pColliderCom), &BoundingDesc)))
		return E_FAIL;*/

	return S_OK;
}

CTerrain_GamePlay* CTerrain_GamePlay::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CTerrain_GamePlay* pInstance = new CTerrain_GamePlay(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CTerrain_GamePlay"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CTerrain_GamePlay::Clone(void* pArg)
{
	CTerrain_GamePlay* pInstance = new CTerrain_GamePlay(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CTerrain_GamePlay"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTerrain_GamePlay::Free()
{

	Safe_Release(m_pNavigationCom);

	for (auto& pTexture : m_pTextureCom)
		Safe_Release(pTexture);

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pVIBufferCom);
	__super::Free();
}