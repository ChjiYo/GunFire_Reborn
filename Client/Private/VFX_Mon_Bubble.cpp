#include "stdafx.h"

#include "VFX_Mon_Bubble.h"
#include "GameInstance.h"

CVFX_Mon_Bubble::CVFX_Mon_Bubble(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CVFX_Monster{ pDevice ,pContext }
{
}

CVFX_Mon_Bubble::CVFX_Mon_Bubble(const CVFX_Mon_Bubble& Prototype)
    :CVFX_Monster{ Prototype }
{
}

HRESULT CVFX_Mon_Bubble::Initialize_Prototype()
{
    if (FAILED(__super::Initialize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CVFX_Mon_Bubble::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(Ready_Components()))
        return E_FAIL;

    return S_OK;
}

void CVFX_Mon_Bubble::Priority_Update(_float fTimeDelta)
{
    __super::Priority_Update(fTimeDelta);
}

void CVFX_Mon_Bubble::Update(_float fTimeDelta)
{
    __super::Update(fTimeDelta);

    m_pVIBufferCom->Go_Dir(fTimeDelta);
}

void CVFX_Mon_Bubble::Late_Update(_float fTimeDelta)
{
    __super::Late_Update(fTimeDelta);


    m_pGameInstance->Add_RenderObject(CRenderer::RG_BLEND, this);
}

HRESULT CVFX_Mon_Bubble::Render()
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

HRESULT CVFX_Mon_Bubble::Ready_Components()
{ /* For.Com_Shader */
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_Particle_VtxPoint"),
        TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
        return E_FAIL;

    /* For.Com_Model */
    if (FAILED(__super::Add_Component(LEVEL_BOSS, TEXT("Prototype_Component_Texture_Skill_Dragon_Floor_Addon"),
        TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
        return E_FAIL;

    /* For.Com_VIBuffer */
    if (FAILED(__super::Add_Component(LEVEL_BOSS, TEXT("Prototype_Component_VIBuffer_Particle_Bubble"),
        TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
        return E_FAIL;

    return S_OK;
}

HRESULT CVFX_Mon_Bubble::Bind_ShaderResources()
{

    if (FAILED(__super::Bind_WorldMatrix(m_pShaderCom, "g_WorldMatrix")))
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

CVFX_Mon_Bubble* CVFX_Mon_Bubble::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CVFX_Mon_Bubble* pInstance = new CVFX_Mon_Bubble(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX(TEXT("Failed to Created : CVFX_Mon_Bubble"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CVFX_Mon_Bubble::Clone(void* pArg)
{
    CVFX_Mon_Bubble* pInstance = new CVFX_Mon_Bubble(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX(TEXT("Failed to Cloned : CVFX_Mon_Bubble"));
        Safe_Release(pInstance);
    }

    return pInstance;
}


void CVFX_Mon_Bubble::Free()
{
    Safe_Release(m_pShaderCom);
    Safe_Release(m_pTextureCom);
    Safe_Release(m_pVIBufferCom);

    __super::Free();
}
