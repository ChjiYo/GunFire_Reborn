#include "stdafx.h"

#include "VFX_Shuriken.h"
#include "GameInstance.h"

#include "Player.h"

CVFX_Shuriken::CVFX_Shuriken(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CVFX{ pDevice , pContext }
{
}

CVFX_Shuriken::CVFX_Shuriken(const CVFX_Shuriken& Prototype)
    :CVFX{ Prototype }
{
}

HRESULT CVFX_Shuriken::Initialize_Prototype()
{
    if (FAILED(__super::Initialize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CVFX_Shuriken::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(Ready_Components()))
        return E_FAIL;

    VFX_DESC* Desc = static_cast<VFX_DESC*>(pArg);
    _vector vPos = Desc->vCreatePos;

    m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);

    return S_OK;
}

void CVFX_Shuriken::Priority_Update(_float fTimeDelta)
{
 
}

void CVFX_Shuriken::Update(_float fTimeDelta)
{
    m_pVIBufferCom->Spread(fTimeDelta);

    m_pTransformCom->Gravity(0.1f, 0.f, fTimeDelta);
}

void CVFX_Shuriken::Late_Update(_float fTimeDelta)
{
    m_pGameInstance->Add_RenderObject(CRenderer::RG_BLEND, this);
}

HRESULT CVFX_Shuriken::Render()
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

HRESULT CVFX_Shuriken::Ready_Components()
{
    /* For.Com_Shader */
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_Particle_VtxPoint"),
        TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
        return E_FAIL;

    /* For.Com_Model */
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_VFX_Shuriken"),
        TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
        return E_FAIL;

    /* For.Com_VIBuffer */
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Particle_Shuriken"),
        TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
        return E_FAIL;

    return S_OK;
}

HRESULT CVFX_Shuriken::Bind_ShaderResources()
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

void CVFX_Shuriken::Destroy(_float fTimeDelta)
{
    m_bDead = true;
}


CVFX_Shuriken* CVFX_Shuriken::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CVFX_Shuriken* pInstance = new CVFX_Shuriken(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX(TEXT("Failed to Created : CVFX_Shuriken"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CVFX_Shuriken::Clone(void* pArg)
{
    CVFX_Shuriken* pInstance = new CVFX_Shuriken(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX(TEXT("Failed to Cloned : CVFX_Shuriken"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CVFX_Shuriken::Free()
{
    Safe_Release(m_pShaderCom);
    Safe_Release(m_pVIBufferCom);
    Safe_Release(m_pTextureCom);

    __super::Free();
}
