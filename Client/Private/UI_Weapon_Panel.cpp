#include "stdafx.h"

#include "UI_Weapon_Panel.h"

CUI_Weapon_Panel::CUI_Weapon_Panel(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CUIObject{ pDevice ,pContext }
{
}

CUI_Weapon_Panel::CUI_Weapon_Panel(const CUI_Weapon_Panel& Prototype)
	:CUIObject{ Prototype }
{
}

HRESULT CUI_Weapon_Panel::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Weapon_Panel::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	//256  , 81
	__super::Set_UI_Setting(384.f, 121.5f, 1400.f, 750.f);

	return S_OK;
}

void CUI_Weapon_Panel::Priority_Update(_float fTimeDelta)
{
}

void CUI_Weapon_Panel::Update(_float fTimeDelta)
{
}

void CUI_Weapon_Panel::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderObject(CRenderer::RG_UI, this);
}

HRESULT CUI_Weapon_Panel::Render()
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

HRESULT CUI_Weapon_Panel::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_Weapon_Panel"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;
}

CUI_Weapon_Panel* CUI_Weapon_Panel::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_Weapon_Panel* pInstatnce = new CUI_Weapon_Panel(pDevice, pContext);

	if (FAILED(pInstatnce->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CUI_Weapon_Panel"));
		Safe_Release(pInstatnce);
	}

	return pInstatnce;
}

CGameObject* CUI_Weapon_Panel::Clone(void* pArg)
{
	CUI_Weapon_Panel* pInstatnce = new CUI_Weapon_Panel(*this);

	if (FAILED(pInstatnce->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloend : CUI_Weapon_Panel"));
		Safe_Release(pInstatnce);
	}

	return pInstatnce;
}

void CUI_Weapon_Panel::Free()
{
	__super::Free();
}
