#include "stdafx.h"

#include "UI_Number.h"
#include "GameInstance.h"

CUI_Number::CUI_Number(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CUIObject{ pDevice ,pContext }
{
}

CUI_Number::CUI_Number(const CUI_Number& Prototype)
    :CUIObject{ Prototype }
{
}

HRESULT CUI_Number::Initialize_Prototype()
{
    if (FAILED(__super::Initialize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CUI_Number::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(Ready_Component()))
        return E_FAIL;

    return S_OK;
}

void CUI_Number::Priority_Update(_float fTimeDelta)
{
    
}

void CUI_Number::Update(_float fTimeDelta)
{
}

void CUI_Number::Late_Update(_float fTimeDelta)
{
    m_pGameInstance->Add_RenderObject(CRenderer::RG_UI, this);
}

HRESULT CUI_Number::Render()
{
    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;

    if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", 0)))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Begin(0)))
        return E_FAIL;

    if (FAILED(m_pVIBufferCom->Bind_Buffers()))
        return E_FAIL;

    if (FAILED(m_pVIBufferCom->Render()))
        return E_FAIL;

    return S_OK;
}

HRESULT CUI_Number::Ready_Component()
{
    if (FAILED(__super::Ready_Components()))
        return E_FAIL;

    /* For.Com_Texture */
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Number"),
        TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
        return E_FAIL;

    return S_OK;
}

HRESULT CUI_Number::Bind_ShaderResources()
{
    if (FAILED(__super::Bind_Perspective()))
        return E_FAIL;

    return S_OK;
}

CUI_Number* CUI_Number::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CUI_Number* pInstance = new CUI_Number(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX(TEXT("Failed to Created : CUI_Cross_Hair"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CUI_Number::Clone(void* pArg)
{
    CUI_Number* pInstance = new CUI_Number(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX(TEXT("Failed to Cloned : CUI_Cross_Hair"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CUI_Number::Free()
{
    __super::Free();
}
