#include "stdafx.h"

#include "UI_Weapon_BBullet.h"

CUI_Weapon_BBullet::CUI_Weapon_BBullet(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CUIObject{ pDevice ,pContext }
{
}

CUI_Weapon_BBullet::CUI_Weapon_BBullet(const CUI_Weapon_BBullet& Prototype)
	:CUIObject{ Prototype }
{
}

HRESULT CUI_Weapon_BBullet::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Weapon_BBullet::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;
	
	__super::Set_UI_Setting(35.f, 35.f, 1350.f, 750.f);

	return S_OK;
}

void CUI_Weapon_BBullet::Priority_Update(_float fTimeDelta)
{
}

void CUI_Weapon_BBullet::Update(_float fTimeDelta)
{
}

void CUI_Weapon_BBullet::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderObject(CRenderer::RG_UI, this);
}

HRESULT CUI_Weapon_BBullet::Render()
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

HRESULT CUI_Weapon_BBullet::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_Weapon_BlueBullet"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;
}

CUI_Weapon_BBullet* CUI_Weapon_BBullet::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_Weapon_BBullet* pInstatnce = new CUI_Weapon_BBullet(pDevice, pContext);

	if (FAILED(pInstatnce->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CUI_Weapon_BBullet"));
		Safe_Release(pInstatnce);
	}

	return pInstatnce;
}

CGameObject* CUI_Weapon_BBullet::Clone(void* pArg)
{
	CUI_Weapon_BBullet* pInstatnce = new CUI_Weapon_BBullet(*this);

	if (FAILED(pInstatnce->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloend : CUI_Weapon_BBullet"));
		Safe_Release(pInstatnce);
	}

	return pInstatnce;
}

void CUI_Weapon_BBullet::Free()
{
	__super::Free();
}
