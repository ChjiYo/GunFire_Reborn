#include "stdafx.h"

#include "UI_NPC_Weapon_Btn.h"


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

	__super::Set_UI_Setting(400.f, 300.f, g_iWinSizeX * 0.5f, g_iWinSizeY * 0.5f -150.f);

	//m_iCoin += rand() % 100 + 10;

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

	return S_OK;
}

HRESULT CUI_NPC_Weapon_Btn::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Item_Coin"),
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
