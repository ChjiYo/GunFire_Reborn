#include "stdafx.h"

#include "VFX_Mon_PosCircle.h"
#include "GameInstance.h"

#include "Player.h"

CVFX_Mon_PosCircle::CVFX_Mon_PosCircle(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CVFX{ pDevice , pContext }
{
}

CVFX_Mon_PosCircle::CVFX_Mon_PosCircle(const CVFX_Mon_PosCircle& Prototype)
    :CVFX{ Prototype }
{
}

HRESULT CVFX_Mon_PosCircle::Initialize_Prototype()
{
    if (FAILED(__super::Initialize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CVFX_Mon_PosCircle::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(Ready_Components()))
        return E_FAIL;

    VFX_DESC* pDesc = static_cast<VFX_DESC*>(pArg);
    m_pOriginTransform = pDesc->pOriginTransform;
  
    Safe_AddRef(m_pOriginTransform);

    m_pTransformCom->Set_Scaled(8.f, 8.f, 8.f);
    m_pTransformCom->Rotation({ 1.f, 0.f , 0.f }, XMConvertToRadians(90.f));

    return S_OK;
}

void CVFX_Mon_PosCircle::Priority_Update(_float fTimeDelta)
{
    m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSetY(m_pOriginTransform->Get_State(CTransform::STATE_POSITION), 0.001f));

}

void CVFX_Mon_PosCircle::Update(_float fTimeDelta)
{

    Destroy(fTimeDelta);
}

void CVFX_Mon_PosCircle::Late_Update(_float fTimeDelta)
{
    m_pGameInstance->Add_RenderObject(CRenderer::RG_BLEND, this);
}

HRESULT CVFX_Mon_PosCircle::Render()
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

HRESULT CVFX_Mon_PosCircle::Ready_Components()
{
    /* For.Com_Shader */
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxPosTex"),
        TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
        return E_FAIL;

    /* For.Com_Model */
    if (FAILED(__super::Add_Component(LEVEL_BOSS, TEXT("Prototype_Component_Texture_VFX_Pos_Circle"),
        TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
        return E_FAIL;

    /* For.Com_VIBuffer */
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"),
        TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
        return E_FAIL;

    return S_OK;
}

HRESULT CVFX_Mon_PosCircle::Bind_ShaderResources()
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

void CVFX_Mon_PosCircle::Destroy(_float fTimeDelta)
{

}


CVFX_Mon_PosCircle* CVFX_Mon_PosCircle::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CVFX_Mon_PosCircle* pInstance = new CVFX_Mon_PosCircle(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX(TEXT("Failed to Created : CVFX_Mon_PosCircle"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CVFX_Mon_PosCircle::Clone(void* pArg)
{
    CVFX_Mon_PosCircle* pInstance = new CVFX_Mon_PosCircle(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX(TEXT("Failed to Cloned : CVFX_Mon_PosCircle"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CVFX_Mon_PosCircle::Free()
{
    Safe_Release(m_pOriginTransform);
    Safe_Release(m_pShaderCom);
    Safe_Release(m_pVIBufferCom);
    Safe_Release(m_pTextureCom);

    __super::Free();
}
