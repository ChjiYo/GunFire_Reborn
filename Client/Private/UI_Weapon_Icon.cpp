#include "stdafx.h"

#include "UI_Weapon_Icon.h"

CUI_Weapon_Icon::CUI_Weapon_Icon(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CUIObject{ pDevice ,pContext }
{
}

CUI_Weapon_Icon::CUI_Weapon_Icon(const CUI_Weapon_Icon& Prototype)
	:CUIObject{ Prototype }
{
}

HRESULT CUI_Weapon_Icon::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Weapon_Icon::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	UI_DESC* Desc = static_cast<UI_DESC*>(pArg);
	m_pPlayer = dynamic_cast<CPlayer*>(Desc->pGameObject);

	Safe_AddRef(m_pPlayer);

	__super::Set_UI_Setting(200.f, 200.f, 1450.f, 750.f);

	return S_OK;
}

void CUI_Weapon_Icon::Priority_Update(_float fTimeDelta)
{
}

void CUI_Weapon_Icon::Update(_float fTimeDelta)
{

}

void CUI_Weapon_Icon::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderObject(CRenderer::RG_UI, this);
}

HRESULT CUI_Weapon_Icon::Render()
{
	if (FAILED(__super::Bind_ShaderResources()))
		return E_FAIL;

<<<<<<< HEAD
	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", m_pPlayer->Get_WeaponID())))
=======
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(m_pGameInstance->Get_Layer(LEVEL_STATIC, TEXT("Layer_Player")).back());

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", pPlayer->Get_WeaponID())))
>>>>>>> Main/Add
		return E_FAIL;

	if (FAILED(m_pShaderCom->Begin(0)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Bind_Buffers()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Weapon_Icon::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Weapon_Icon"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;
}

CUI_Weapon_Icon* CUI_Weapon_Icon::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_Weapon_Icon* pInstatnce = new CUI_Weapon_Icon(pDevice, pContext);

	if (FAILED(pInstatnce->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CUI_Weapon_Icon"));
		Safe_Release(pInstatnce);
	}

	return pInstatnce;
}

CGameObject* CUI_Weapon_Icon::Clone(void* pArg)
{
	CUI_Weapon_Icon* pInstatnce = new CUI_Weapon_Icon(*this);

	if (FAILED(pInstatnce->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloend : CUI_Weapon_Icon"));
		Safe_Release(pInstatnce);
	}

	return pInstatnce;
}

void CUI_Weapon_Icon::Free()
{
	Safe_Release(m_pPlayer);

	__super::Free();
}
