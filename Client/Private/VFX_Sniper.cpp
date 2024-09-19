#include "stdafx.h"

#include "VFX_Sniper.h"
#include "GameInstance.h"

#include "Player.h"

CVFX_Sniper::CVFX_Sniper(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CVFX{ pDevice , pContext }
{
}

CVFX_Sniper::CVFX_Sniper(const CVFX_Sniper& Prototype)
    :CVFX{ Prototype }
{
}

HRESULT CVFX_Sniper::Initialize_Prototype()
{
    if (FAILED(__super::Initialize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CVFX_Sniper::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(Ready_Components()))
        return E_FAIL;

    VFX_DESC* Desc = static_cast<VFX_DESC*>(pArg);
    m_pOriginTransform = Desc->pOriginTransform;
   
    _vector vStartPos = m_pOriginTransform->Get_State(CTransform::STATE_POSITION);
    _vector vEndPos = Desc->vCreatePos;
    
   Safe_AddRef(m_pOriginTransform);

    _float fDist = XMVectorGetX(XMVector4Length(vEndPos - vStartPos));
    
    m_pTransformCom->Set_WorldMatrix(m_pOriginTransform->Get_WorldMatrix());
    
    _vector vPos = m_pOriginTransform->Get_State(CTransform::STATE_POSITION);
    _vector vUp = m_pOriginTransform->Get_State(CTransform::STATE_UP);
    _vector vLook = m_pOriginTransform->Get_State(CTransform::STATE_LOOK);
    
    vPos += (vLook * (fDist * 0.5f + 0.45f)) + (vUp *0.2f);
    
    m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);
    m_pTransformCom->Set_Scaled(fDist, 0.25f, 1.f);
    
    m_pTransformCom->Radian_Turn(m_pTransformCom->Get_State(CTransform::STATE_UP), XMConvertToRadians(90.f));

    return S_OK;
}

void CVFX_Sniper::Priority_Update(_float fTimeDelta)
{
}

void CVFX_Sniper::Update(_float fTimeDelta)
{

   m_fTimer += fTimeDelta;
   if (m_fTimer >= 1.f)
   {
      Destroy(fTimeDelta);
       m_fTimer = 0.f;
   }
}

void CVFX_Sniper::Late_Update(_float fTimeDelta)
{
    m_pGameInstance->Add_RenderObject(CRenderer::RG_BLEND, this);
}

HRESULT CVFX_Sniper::Render()
{
    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Begin(2)))
        return E_FAIL;

    if (FAILED(m_pVIBufferCom->Bind_Buffers()))
        return E_FAIL;

    if (FAILED(m_pVIBufferCom->Render()))
        return E_FAIL;


    return S_OK;
}

HRESULT CVFX_Sniper::Ready_Components()
{
    /* For.Com_Shader */
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_Effect_MaskTex"),
        TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
        return E_FAIL;

    /* For.Com_Model */
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_VFX_Sniper"),
        TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
        return E_FAIL;

    /* For.Com_VIBuffer */
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"),
        TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
        return E_FAIL;

    return S_OK;
}

HRESULT CVFX_Sniper::Bind_ShaderResources()
{
    if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
        return E_FAIL;

    if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", 0)))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_RawValue("g_Timer", &m_fTimer, sizeof(_float))))
        return E_FAIL;

    return S_OK;
}

void CVFX_Sniper::Destroy(_float fTimeDelta)
{
   m_bDead = true;
}


CVFX_Sniper* CVFX_Sniper::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CVFX_Sniper* pInstance = new CVFX_Sniper(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX(TEXT("Failed to Created : CVFX_Sniper"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CVFX_Sniper::Clone(void* pArg)
{
    CVFX_Sniper* pInstance = new CVFX_Sniper(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX(TEXT("Failed to Cloned : CVFX_Sniper"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CVFX_Sniper::Free()
{
    Safe_Release(m_pShaderCom);
    Safe_Release(m_pVIBufferCom);
    Safe_Release(m_pTextureCom);
    Safe_Release(m_pOriginTransform);

    __super::Free();
}
