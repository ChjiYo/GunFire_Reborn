#include "stdafx.h"

#include "UI_Weapon_NumPanel.h"

CUI_Weapon_NumPanel::CUI_Weapon_NumPanel(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CUIObject{ pDevice ,pContext }
{
}

CUI_Weapon_NumPanel::CUI_Weapon_NumPanel(const CUI_Weapon_NumPanel& Prototype)
	:CUIObject{ Prototype }
{
}

HRESULT CUI_Weapon_NumPanel::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Weapon_NumPanel::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	__super::Set_UI_Setting(50.f, 50.f, 1500.f, 750.f);

	return S_OK;
}

void CUI_Weapon_NumPanel::Priority_Update(_float fTimeDelta)
{
}

void CUI_Weapon_NumPanel::Update(_float fTimeDelta)
{
}

void CUI_Weapon_NumPanel::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderObject(CRenderer::RG_UI, this);
}

HRESULT CUI_Weapon_NumPanel::Render()
{
	if (FAILED(__super::Bind_ShaderResources()))
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

HRESULT CUI_Weapon_NumPanel::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_Weapon_NumPanel"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;
}

CUI_Weapon_NumPanel* CUI_Weapon_NumPanel::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_Weapon_NumPanel* pInstatnce = new CUI_Weapon_NumPanel(pDevice, pContext);

	if (FAILED(pInstatnce->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CUI_Weapon_NumPanel"));
		Safe_Release(pInstatnce);
	}

	return pInstatnce;
}

CGameObject* CUI_Weapon_NumPanel::Clone(void* pArg)
{
	CUI_Weapon_NumPanel* pInstatnce = new CUI_Weapon_NumPanel(*this);

	if (FAILED(pInstatnce->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloend : CUI_Weapon_NumPanel"));
		Safe_Release(pInstatnce);
	}

	return pInstatnce;
}

void CUI_Weapon_NumPanel::Free()
{
	__super::Free();
}
