#include "stdafx.h"

#include "UI_Weapon_GBullet.h"

CUI_Weapon_GBullet::CUI_Weapon_GBullet(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CUIObject{ pDevice ,pContext }
{
}

CUI_Weapon_GBullet::CUI_Weapon_GBullet(const CUI_Weapon_GBullet& Prototype)
	:CUIObject{ Prototype }
{
}

HRESULT CUI_Weapon_GBullet::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Weapon_GBullet::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	__super::Set_UI_Setting(35.f, 35.f, 1300.f, 750.f);

	return S_OK;
}

void CUI_Weapon_GBullet::Priority_Update(_float fTimeDelta)
{
}

void CUI_Weapon_GBullet::Update(_float fTimeDelta)
{
}

void CUI_Weapon_GBullet::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderObject(CRenderer::RG_UI, this);
}

HRESULT CUI_Weapon_GBullet::Render()
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

HRESULT CUI_Weapon_GBullet::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_Weapon_GreenBullet"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;
}

CUI_Weapon_GBullet* CUI_Weapon_GBullet::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_Weapon_GBullet* pInstatnce = new CUI_Weapon_GBullet(pDevice, pContext);

	if (FAILED(pInstatnce->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CUI_Weapon_GBullet"));
		Safe_Release(pInstatnce);
	}

	return pInstatnce;
}

CGameObject* CUI_Weapon_GBullet::Clone(void* pArg)
{
	CUI_Weapon_GBullet* pInstatnce = new CUI_Weapon_GBullet(*this);

	if (FAILED(pInstatnce->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloend : CUI_Weapon_GBullet"));
		Safe_Release(pInstatnce);
	}

	return pInstatnce;
}

void CUI_Weapon_GBullet::Free()
{
	__super::Free();
}
