#include "stdafx.h"

#include "Cube.h"
#include "GameInstance.h"

CCube::CCube(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CElement{ pDevice ,pContext }
{
}

CCube::CCube(const CCube& Prototype)
	:CElement{ Prototype }
{
}

HRESULT CCube::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CCube::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if(FAILED(Ready_Components()))
		return E_FAIL;

	//_float3 vPos = XMStoreFloat3(m_pTransformCom->Get_State(CTransform::STATE_POSITION));

	return S_OK;
}

void CCube::Priority_Update(_float fTimeDelta)
{
	int a = 10;
}

void CCube::Update(_float fTimeDelta)
{

}
void CCube::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderObject(CRenderer::RG_NONBLEND, this);
}

HRESULT CCube::Render()
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

HRESULT CCube::Ready_Components()
{
	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxNorTex"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Black"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Cube"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CCube::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	/*if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", 0)))
		return E_FAIL;*/

	return S_OK;
}

CCube* CCube::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CCube* pInstance = new CCube(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CCube"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CCube::Clone(void* pArg)
{
	CCube* pInstance = new CCube(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CCube"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCube::Free()
{
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);

	__super::Free();
}
