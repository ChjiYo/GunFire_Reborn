#include "stdafx.h"

#include "VFX_Shield_Hit.h"
#include "GameInstance.h"

#include "Player.h"

CVFX_Shield_Hit::CVFX_Shield_Hit(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CVFX{ pDevice , pContext }
{
}

CVFX_Shield_Hit::CVFX_Shield_Hit(const CVFX_Shield_Hit& Prototype)
    :CVFX{ Prototype }
{
}

HRESULT CVFX_Shield_Hit::Initialize_Prototype()
{
    if (FAILED(__super::Initialize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CVFX_Shield_Hit::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(Ready_Components()))
        return E_FAIL;

    VFX_DESC* Desc = static_cast<VFX_DESC*>(pArg);
    _vector _vCreatePos = Desc->vCreatePos;

    _float fPosY = XMVectorGetY(_vCreatePos);

    m_pTransformCom->Set_State(CTransform::STATE_POSITION, _vCreatePos);
    m_pTransformCom->Set_Scaled(1.f, 1.f, 1.f);

    CTransform* pPlayerTransform = dynamic_cast<CTransform*>(m_pGameInstance->Get_Component(m_pGameInstance->GetCurrentLevelIndex(), TEXT("Layer_Player"), TEXT("Com_Transform")));
    m_pTransformCom->LookAt(pPlayerTransform->Get_State(CTransform::STATE_POSITION));
    m_pTransformCom->Radian_Turn(m_pTransformCom->Get_State(CTransform::STATE_LOOK), XMConvertToRadians(180.f));
    m_pTransformCom->Radian_Turn(m_pTransformCom->Get_State(CTransform::STATE_UP), XMConvertToRadians(315.f));

    return S_OK;
}

void CVFX_Shield_Hit::Priority_Update(_float fTimeDelta)
{
    m_fAnimFream += fTimeDelta * 12.f;

    if (m_fAnimFream >= 3.f)
    {
        Destroy(fTimeDelta);
        m_fAnimFream = 0.f;
    }



}

void CVFX_Shield_Hit::Update(_float fTimeDelta)
{

}

void CVFX_Shield_Hit::Late_Update(_float fTimeDelta)
{
    m_pGameInstance->Add_RenderObject(CRenderer::RG_BLEND, this);
}

HRESULT CVFX_Shield_Hit::Render()
{
    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Begin(5)))
        return E_FAIL;

    if (FAILED(m_pVIBufferCom->Bind_Buffers()))
        return E_FAIL;

    if (FAILED(m_pVIBufferCom->Render()))
        return E_FAIL;


    return S_OK;
}

HRESULT CVFX_Shield_Hit::Ready_Components()
{
    /* For.Com_Shader */
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxPosTex"),
        TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
        return E_FAIL;

    /* For.Com_Model */
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Shield_Hit"),
        TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
        return E_FAIL;

    /* For.Com_VIBuffer */
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"),
        TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
        return E_FAIL;

    return S_OK;
}

HRESULT CVFX_Shield_Hit::Bind_ShaderResources()
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

void CVFX_Shield_Hit::Destroy(_float fTimeDelta)
{
    m_bDead = true;
}


CVFX_Shield_Hit* CVFX_Shield_Hit::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CVFX_Shield_Hit* pInstance = new CVFX_Shield_Hit(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX(TEXT("Failed to Created : CVFX_Shield_Hit"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CVFX_Shield_Hit::Clone(void* pArg)
{
    CVFX_Shield_Hit* pInstance = new CVFX_Shield_Hit(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX(TEXT("Failed to Cloned : CVFX_Shield_Hit"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CVFX_Shield_Hit::Free()
{
    Safe_Release(m_pShaderCom);
    Safe_Release(m_pVIBufferCom);
    Safe_Release(m_pTextureCom);

    __super::Free();
}
