#include "stdafx.h"

#include "VFX_Pistol_Barrel.h"
#include "GameInstance.h"

#include "Player.h"

CVFX_Pistol_Barrel::CVFX_Pistol_Barrel(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CVFX{ pDevice , pContext }
{
}

CVFX_Pistol_Barrel::CVFX_Pistol_Barrel(const CVFX_Pistol_Barrel& Prototype)
    :CVFX{ Prototype }
{
}

HRESULT CVFX_Pistol_Barrel::Initialize_Prototype()
{
    if (FAILED(__super::Initialize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CVFX_Pistol_Barrel::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(Ready_Components()))
        return E_FAIL;

    VFX_DESC* Desc = static_cast<VFX_DESC*>(pArg);
    m_pOriginTransform = Desc->pOriginTransform;
    Safe_Release(m_pOriginTransform);

    _vector vPos = m_pOriginTransform->Get_State(CTransform::STATE_POSITION);
    _vector vRight = m_pOriginTransform->Get_State(CTransform::STATE_RIGHT);
    _vector vUp = m_pOriginTransform->Get_State(CTransform::STATE_UP);
    _vector vLook = m_pOriginTransform->Get_State(CTransform::STATE_LOOK);

    vPos += (vLook * 1.f);
    
    m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);

    m_pTransformCom->Rotation({ 0.f, 1.f , 0.f }, XMConvertToRadians(90.f));

    return S_OK;
}

void CVFX_Pistol_Barrel::Priority_Update(_float fTimeDelta)
{
    m_fAnimFream += fTimeDelta * 14.f;

    if (m_fAnimFream >= 7.f)
    {
        Destroy(fTimeDelta);
        m_fAnimFream = 0.f;
    }
}

void CVFX_Pistol_Barrel::Update(_float fTimeDelta)
{

}

void CVFX_Pistol_Barrel::Late_Update(_float fTimeDelta)
{
    m_pGameInstance->Add_RenderObject(CRenderer::RG_BLEND, this);
}

HRESULT CVFX_Pistol_Barrel::Render()
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

HRESULT CVFX_Pistol_Barrel::Ready_Components()
{
    /* For.Com_Shader */
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxPosTex"),
        TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
        return E_FAIL;

    /* For.Com_Model */
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Barrel"),
        TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
        return E_FAIL;

    /* For.Com_VIBuffer */
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"),
        TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
        return E_FAIL;

    return S_OK;
}

HRESULT CVFX_Pistol_Barrel::Bind_ShaderResources()
{
    if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
        return E_FAIL;

    if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", m_fAnimFream)))
        return E_FAIL;

    return S_OK;
}

void CVFX_Pistol_Barrel::Destroy(_float fTimeDelta)
{
    m_bDead = true;
}


CVFX_Pistol_Barrel* CVFX_Pistol_Barrel::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CVFX_Pistol_Barrel* pInstance = new CVFX_Pistol_Barrel(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX(TEXT("Failed to Created : CVFX_Pistol_Barrel"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CVFX_Pistol_Barrel::Clone(void* pArg)
{
    CVFX_Pistol_Barrel* pInstance = new CVFX_Pistol_Barrel(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX(TEXT("Failed to Cloned : CVFX_Pistol_Barrel"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CVFX_Pistol_Barrel::Free()
{
    Safe_Release(m_pShaderCom);
    Safe_Release(m_pVIBufferCom);
    Safe_Release(m_pTextureCom);
    Safe_Release(m_pOriginTransform);

    __super::Free();
}
