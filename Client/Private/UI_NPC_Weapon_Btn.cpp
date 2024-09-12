#include "stdafx.h"

#include "UI_NPC_Weapon_Btn.h"
#include "Player.h"

#include"UI_Coin.h"


CUI_NPC_Weapon_Btn::CUI_NPC_Weapon_Btn(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CUIObject{ pDevice ,pContext }
{
}

CUI_NPC_Weapon_Btn::CUI_NPC_Weapon_Btn(const CUI_NPC_Weapon_Btn& Prototype)
	:CUIObject{ Prototype }
{
}

HRESULT CUI_NPC_Weapon_Btn::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_NPC_Weapon_Btn::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_iNumber = static_cast<UI_DESC*>(pArg)->iNumUI;

	if(m_iNumber == 0)
		m_fPosX = g_iWinSizeX * 0.5f - 250;
	else
		m_fPosX = g_iWinSizeX * 0.5f + 250;

	m_fSizeX = 180.f;
	m_fSizeY = 40.f;
	//m_fPosX = g_iWinSizeX * 0.5f;
	m_fPosY = g_iWinSizeY * 0.5f + 250.f;

	__super::Set_UI_Setting(m_fSizeX, m_fSizeY, m_fPosX, m_fPosY);

	return S_OK;

}

void CUI_NPC_Weapon_Btn::Priority_Update(_float fTimeDelta)
{

}

void CUI_NPC_Weapon_Btn::Update(_float fTimeDelta)
{
	if (m_pGameInstance->Get_DIKeyState(DIK_ESCAPE))
		m_bDead = true;
}

void CUI_NPC_Weapon_Btn::Late_Update(_float fTimeDelta)
{
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(m_pGameInstance->Get_Layer(LEVEL_GAMEPLAY, TEXT("Layer_Player")).back());
	if (ClickRange() && m_pGameInstance->Key_Down(VK_LBUTTON))
	{
		if(m_iNumber == 0)
			pPlayer->WeaponUpgread(CPlayer::ONE);
		else 
			pPlayer->WeaponUpgread(CPlayer::TWO);
	}

	m_pGameInstance->Add_RenderObject(CRenderer::RG_UI, this);
}

HRESULT CUI_NPC_Weapon_Btn::Render()
{
	if (FAILED(__super::Bind_ShaderResources()))
		return E_FAIL;;

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", 0)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Begin(0)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Bind_Buffers()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	
	m_pGameInstance->Draw_Font(TEXT("Font_Nexon"), TEXT(" °­ È­ "), _float2(m_fPosX - 50, m_fPosY- 15), { 0.6235, 0.5725, 0.4353, 1.f }, 0.f, _float2(0.f, 0.f), 0.5f);

	return S_OK;
}

HRESULT CUI_NPC_Weapon_Btn::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_NPC_Weapon_Btn"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;
}

CUI_NPC_Weapon_Btn* CUI_NPC_Weapon_Btn::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_NPC_Weapon_Btn* pInstatnce = new CUI_NPC_Weapon_Btn(pDevice, pContext);

	if (FAILED(pInstatnce->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CUI_NPC_Weapon_Btn"));
		Safe_Release(pInstatnce);
	}

	return pInstatnce;
}

CGameObject* CUI_NPC_Weapon_Btn::Clone(void* pArg)
{
	CUI_NPC_Weapon_Btn* pInstatnce = new CUI_NPC_Weapon_Btn(*this);

	if (FAILED(pInstatnce->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloend : CUI_NPC_Weapon_Btn"));
		Safe_Release(pInstatnce);
	}

	return pInstatnce;
}

void CUI_NPC_Weapon_Btn::Free()
{
	__super::Free();
}
