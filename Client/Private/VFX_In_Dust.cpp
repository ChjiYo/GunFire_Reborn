#include "stdafx.h"

#include "VFX_In_Dust.h"
#include "GameInstance.h"

CVFX_In_Dust::CVFX_In_Dust(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CVFX{ pDevice ,pContext }
{
}

CVFX_In_Dust::CVFX_In_Dust(const CVFX_In_Dust& Prototype)
    :CVFX{ Prototype }
{
}

HRESULT CVFX_In_Dust::Initialize_Prototype()
{
    if (FAILED(__super::Initialize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CVFX_In_Dust::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(Ready_Components()))
        return E_FAIL;

    VFX_DESC* pDesc = static_cast<VFX_DESC*>(pArg);
    
    m_pTransformCom->Set_WorldMatrix(pDesc->pOriginTransform->Get_WorldMatrix());

    return S_OK;
}

void CVFX_In_Dust::Priority_Update(_float fTimeDelta)
{
    __super::Priority_Update(fTimeDelta);
  
}

void CVFX_In_Dust::Update(_float fTimeDelta)
{
    __super::Update(fTimeDelta);

    m_pVIBufferCom->Go_Dir(fTimeDelta);

    m_fScaled += fTimeDelta * 10.f;

    m_pTransformCom->Set_Scaled(1.f, m_fScaled, m_fScaled);
}

void CVFX_In_Dust::Late_Update(_float fTimeDelta)
{
    __super::Late_Update(fTimeDelta);

    m_pGameInstance->Add_RenderObject(CRenderer::RG_BLEND, this);
}

HRESULT CVFX_In_Dust::Render()
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

HRESULT CVFX_In_Dust::Ready_Components()
{ /* For.Com_Shader */
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_Particle_VtxPoint"),
        TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
        return E_FAIL;

    /* For.Com_Model */
    if (FAILED(__super::Add_Component(LEVEL_BOSS, TEXT("Prototype_Component_Texture_VFX_In_Dust"),
        TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
        return E_FAIL;

    /* For.Com_VIBuffer */
    if (FAILED(__super::Add_Component(LEVEL_BOSS, TEXT("Prototype_Component_VIBuffer_Particle_In_Dust"),
        TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
        return E_FAIL;

    return S_OK;
}

HRESULT CVFX_In_Dust::Bind_ShaderResources()
{

    if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
        return E_FAIL;

    if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", 0)))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_RawValue("g_vCamPosition", &m_pGameInstance->Get_CamPosition_Float4(), sizeof(_float4))))
        return E_FAIL;

    return S_OK;
}

CVFX_In_Dust* CVFX_In_Dust::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CVFX_In_Dust* pInstance = new CVFX_In_Dust(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX(TEXT("Failed to Created : CVFX_In_Dust"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CVFX_In_Dust::Clone(void* pArg)
{
    CVFX_In_Dust* pInstance = new CVFX_In_Dust(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX(TEXT("Failed to Cloned : CVFX_In_Dust"));
        Safe_Release(pInstance);
    }

    return pInstance;
}


void CVFX_In_Dust::Free()
{
    Safe_Release(m_pShaderCom);
    Safe_Release(m_pTextureCom);
    Safe_Release(m_pVIBufferCom);

    __super::Free();
}
