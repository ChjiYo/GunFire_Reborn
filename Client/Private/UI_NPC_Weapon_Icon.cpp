#include "stdafx.h"

#include "UI_NPC_Weapon_Icon.h"

CUI_NPC_Weapon_Icon::CUI_NPC_Weapon_Icon(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CUIObject{ pDevice ,pContext }
{
}

CUI_NPC_Weapon_Icon::CUI_NPC_Weapon_Icon(const CUI_NPC_Weapon_Icon& Prototype)
	:CUIObject{ Prototype }
{
}

HRESULT CUI_NPC_Weapon_Icon::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_NPC_Weapon_Icon::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_pPlayer = dynamic_cast<CPlayer*>(m_pGameInstance->Get_Layer(m_pGameInstance->GetLoadingLevelIndex(), TEXT("Layer_Player")).back());
	Safe_AddRef(m_pPlayer);

	__super::Set_UI_Setting(150.f, 150.f, g_iWinSizeX * 0.5f - 100.f, g_iWinSizeY * 0.5f - 225.f);

	return S_OK;
}

void CUI_NPC_Weapon_Icon::Priority_Update(_float fTimeDelta)
{
}

void CUI_NPC_Weapon_Icon::Update(_float fTimeDelta)
{

}

void CUI_NPC_Weapon_Icon::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderObject(CRenderer::RG_UI, this);
}

HRESULT CUI_NPC_Weapon_Icon::Render()
{
	if (FAILED(__super::Bind_ShaderResources()))
		return E_FAIL;

	CPlayer::WEAPONID pID = m_pPlayer->Get_WeaponID(0);
	if (m_pPlayer->Get_WeaponID(0) == CPlayer::WEAPON_END)
		pID = CPlayer::PISTOL;

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", pID)))

		return E_FAIL;

	if (FAILED(m_pShaderCom->Begin(0)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Bind_Buffers()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	switch (pID)
	{
	case CPlayer::PISTOL:
		m_pGameInstance->Draw_Font(TEXT("Font_Nexon"), TEXT("피스톨"), _float2(g_iWinSizeX * 0.5f - 50.f, g_iWinSizeY * 0.5f - 275.f), {0.6235, 0.5725, 0.4353, 1.f} , 0.f, _float2(0.f, 0.f),0.75f);
		break;

	case CPlayer::SHURIKEN:
		m_pGameInstance->Draw_Font(TEXT("Font_Nexon"), TEXT("수리검"), _float2(g_iWinSizeX * 0.5f - 50.f, g_iWinSizeY * 0.5f - 275.f), { 0.6235, 0.5725, 0.4353, 1.f }, 0.f, _float2(0.f, 0.f), 0.75f);
		break;

	case CPlayer::SNIPER:
		m_pGameInstance->Draw_Font(TEXT("Font_Nexon"), TEXT("스나이퍼"), _float2(g_iWinSizeX * 0.5f - 50.f, g_iWinSizeY * 0.5f - 275.f), { 0.6235, 0.5725, 0.4353, 1.f }, 0.f, _float2(0.f, 0.f), 0.75f);
		break;

	case CPlayer::BOOMGUN:
		m_pGameInstance->Draw_Font(TEXT("Font_Nexon"), TEXT("돌격총"), _float2(g_iWinSizeX * 0.5f - 50.f, g_iWinSizeY * 0.5f - 275.f), { 0.6235, 0.5725, 0.4353, 1.f }, 0.f, _float2(0.f, 0.f), 0.75f);
		break;

	default:
		break;
	}

	return S_OK;
}

HRESULT CUI_NPC_Weapon_Icon::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Weapon_Icon"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;
}

CUI_NPC_Weapon_Icon* CUI_NPC_Weapon_Icon::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_NPC_Weapon_Icon* pInstatnce = new CUI_NPC_Weapon_Icon(pDevice, pContext);

	if (FAILED(pInstatnce->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CUI_NPC_Weapon_Icon"));
		Safe_Release(pInstatnce);
	}

	return pInstatnce;
}

CGameObject* CUI_NPC_Weapon_Icon::Clone(void* pArg)
{
	CUI_NPC_Weapon_Icon* pInstatnce = new CUI_NPC_Weapon_Icon(*this);

	if (FAILED(pInstatnce->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloend : CUI_NPC_Weapon_Icon"));
		Safe_Release(pInstatnce);
	}

	return pInstatnce;
}

void CUI_NPC_Weapon_Icon::Free()
{
	Safe_Release(m_pPlayer);

	__super::Free();
}
