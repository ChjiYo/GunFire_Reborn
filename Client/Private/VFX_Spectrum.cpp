#include "stdafx.h"

#include "VFX_Spectrum.h"
#include "GameInstance.h"

CVFX_Spectrum::CVFX_Spectrum(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CVFX_Monster{ pDevice ,pContext }
{
}

CVFX_Spectrum::CVFX_Spectrum(const CVFX_Spectrum& Prototype)
    :CVFX_Monster{ Prototype }
{
}

HRESULT CVFX_Spectrum::Initialize_Prototype()
{
    if (FAILED(__super::Initialize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CVFX_Spectrum::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(Ready_Components()))
        return E_FAIL;

    return S_OK;
}

void CVFX_Spectrum::Priority_Update(_float fTimeDelta)
{
    m_fTimer += fTimeDelta;
    if (m_fTimer >= 1.f)
        m_fTimer = 0.f;

    __super::Priority_Update(fTimeDelta);
}

void CVFX_Spectrum::Update(_float fTimeDelta)
{
    __super::Update(fTimeDelta);
}

void CVFX_Spectrum::Late_Update(_float fTimeDelta)
{
    __super::Late_Update(fTimeDelta);

    m_pGameInstance->Add_RenderObject(CRenderer::RG_BLEND, this);
}

HRESULT CVFX_Spectrum::Render()
{
   if (FAILED(Bind_ShaderResources()))
       return E_FAIL;
   
   _uint	iNumMeshes = m_pModelCom->Get_NumMeshes();
   
   for (size_t i = 0; i < iNumMeshes; i++)
   {
       if (FAILED(m_pModelCom->Bind_MaterialSRV(m_pShaderCom, aiTextureType_DIFFUSE, "g_DiffuseTexture", i)))
           return E_FAIL;
   
       if (FAILED(m_pModelCom->Bind_BoneMatrices(m_pShaderCom, "g_BoneMatrices", (_uint)i)))
           return E_FAIL;
   
       if (FAILED(m_pShaderCom->Begin(4)))
           return E_FAIL;
   
       if (FAILED(m_pModelCom->Render(i)))
           return E_FAIL;
   }
  

    return S_OK;
}

HRESULT CVFX_Spectrum::Ready_Components()
{ /* For.Com_Shader */
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxAnimMesh"),
        TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
        return E_FAIL;

    /* For.Com_Model */
    if (FAILED(__super::Add_Component(m_pGameInstance->GetLoadingLevelIndex(), TEXT("Prototype_Component_Model_Monster_Archer"),
        TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pModelCom))))
        return E_FAIL;

    return S_OK;
}

HRESULT CVFX_Spectrum::Bind_ShaderResources()
{
    if (FAILED(__super::Bind_WorldMatrix(m_pShaderCom, "g_WorldMatrix")))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_RawValue("g_Timer", &m_fTimer, sizeof(_float))))
        return E_FAIL;

    return S_OK;
}

CVFX_Spectrum* CVFX_Spectrum::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CVFX_Spectrum* pInstance = new CVFX_Spectrum(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX(TEXT("Failed to Created : CVFX_Spectrum"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CVFX_Spectrum::Clone(void* pArg)
{
    CVFX_Spectrum* pInstance = new CVFX_Spectrum(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX(TEXT("Failed to Cloned : CVFX_Spectrum"));
        Safe_Release(pInstance);
    }

    return pInstance;
}


void CVFX_Spectrum::Free()
{
    Safe_Release(m_pShaderCom);


    __super::Free();
}
