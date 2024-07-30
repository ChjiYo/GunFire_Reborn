#include "stdafx.h"
#include "..\Public\Effect_Deco_Snow.h"

#include "GameInstance.h"

CEffect_Deco_Snow::CEffect_Deco_Snow(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject{ pDevice, pContext }
{

}

CEffect_Deco_Snow::CEffect_Deco_Snow(const CEffect_Deco_Snow& Prototype)
	: CGameObject{ Prototype }
{

}

HRESULT CEffect_Deco_Snow::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CEffect_Deco_Snow::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	return S_OK;
}

void CEffect_Deco_Snow::Priority_Update(_float fTimeDelta)
{
}

void CEffect_Deco_Snow::Update(_float fTimeDelta)
{
	m_pVIBufferCom->Spread(fTimeDelta);
}

void CEffect_Deco_Snow::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderObject(CRenderer::RG_NONBLEND, this);
}

HRESULT CEffect_Deco_Snow::Render()
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

HRESULT CEffect_Deco_Snow::Ready_Components()
{
	/* Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_Particle_VtxRect"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Snow"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;


	/* Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_VIBuffer_Rect_Instancing"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CEffect_Deco_Snow::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", 0)))
		return E_FAIL;

	return S_OK;
}

CEffect_Deco_Snow* CEffect_Deco_Snow::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CEffect_Deco_Snow* pInstance = new CEffect_Deco_Snow(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CEffect_Deco_Snow"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CEffect_Deco_Snow::Clone(void* pArg)
{
	CEffect_Deco_Snow* pInstance = new CEffect_Deco_Snow(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CEffect_Deco_Snows"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEffect_Deco_Snow::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pVIBufferCom);
}
