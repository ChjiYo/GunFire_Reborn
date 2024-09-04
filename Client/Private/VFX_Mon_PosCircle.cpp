#include "stdafx.h"

#include "VFX_Mon_PosCircle.h"
#include "GameInstance.h"

CVFX_Mon_PosCircle::CVFX_Mon_PosCircle(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CVFX_Monster{ pDevice ,pContext }
{
}

CVFX_Mon_PosCircle::CVFX_Mon_PosCircle(const CVFX_Mon_PosCircle& Prototype)
    :CVFX_Monster{ Prototype }
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

    return S_OK;
}

void CVFX_Mon_PosCircle::Priority_Update(_float fTimeDelta)
{
    __super::Priority_Update(fTimeDelta);

    m_fAnimFrame += (rand() % 26) * fTimeDelta;

    if (m_fAnimFrame >= 13.f)
        m_bDead = true;
}

void CVFX_Mon_PosCircle::Update(_float fTimeDelta)
{
    __super::Update(fTimeDelta);

    m_pVIBufferCom->Spread(fTimeDelta);
}

void CVFX_Mon_PosCircle::Late_Update(_float fTimeDelta)
{
    __super::Late_Update(fTimeDelta);


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
{ /* For.Com_Shader */
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_Particle_VtxPoint"),
        TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
        return E_FAIL;

    /* For.Com_Model */
    if (FAILED(__super::Add_Component(LEVEL_BOSS, TEXT("Prototype_Component_Texture_VFX_Pos_Circle"),
        TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
        return E_FAIL;

    /* For.Com_VIBuffer */
    if (FAILED(__super::Add_Component(LEVEL_BOSS, TEXT("Prototype_Component_VIBuffer_Particle_Circle"),
        TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
        return E_FAIL;

    return S_OK;
}

HRESULT CVFX_Mon_PosCircle::Bind_ShaderResources()
{

    if (FAILED(__super::Bind_WorldMatrix(m_pShaderCom, "g_WorldMatrix")))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
        return E_FAIL;

    if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", m_fAnimFrame)))
        return E_FAIL;


    return S_OK;
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
    Safe_Release(m_pShaderCom);
    Safe_Release(m_pTextureCom);
    Safe_Release(m_pVIBufferCom);

    __super::Free();
}
