#include "stdafx.h"

#include "UI_Bullet_Font.h"
#include "GameInstance.h"

CUI_Bullet_Font::CUI_Bullet_Font(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CGameObject { pDevice ,pContext }
{
}

CUI_Bullet_Font::CUI_Bullet_Font(const CUI_Bullet_Font& Prototype)
    :CGameObject{ Prototype }
{
}

HRESULT CUI_Bullet_Font::Initialize_Prototype()
{
    if (FAILED(__super::Initialize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CUI_Bullet_Font::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    return S_OK;
}

void CUI_Bullet_Font::Priority_Update(_float fTimeDelta)
{
    
}

void CUI_Bullet_Font::Update(_float fTimeDelta)
{
}

void CUI_Bullet_Font::Late_Update(_float fTimeDelta)
{
    m_pGameInstance->Add_RenderObject(CRenderer::RG_UI, this);
}

HRESULT CUI_Bullet_Font::Render()
{
    return S_OK;
}

HRESULT CUI_Bullet_Font::Ready_Fonts()
{


    return S_OK;
}

CUI_Bullet_Font* CUI_Bullet_Font::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CUI_Bullet_Font* pInstance = new CUI_Bullet_Font(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX(TEXT("Failed to Created : CUI_Cross_Hair"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CUI_Bullet_Font::Clone(void* pArg)
{
    CUI_Bullet_Font* pInstance = new CUI_Bullet_Font(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX(TEXT("Failed to Cloned : CUI_Cross_Hair"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CUI_Bullet_Font::Free()
{
    __super::Free();
}
