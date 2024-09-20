#include "stdafx.h"

#include "UI_Coin.h"


CUI_Coin::CUI_Coin(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CUIObject{ pDevice ,pContext }
{
}

CUI_Coin::CUI_Coin(const CUI_Coin& Prototype)
	:CUIObject{ Prototype }
{
}

HRESULT CUI_Coin::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Coin::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	__super::Set_UI_Setting(50.f, 50.f, 1450.f, 50.f);

	return S_OK;
}

void CUI_Coin::Priority_Update(_float fTimeDelta)
{

}

void CUI_Coin::Update(_float fTimeDelta)
{

	if (m_bAddCoin)
	{
		m_iCoin < m_iCopyCoin ? m_iCoin++ : m_iCoin--;

		if (m_iCoin < m_iCopyCoin)
		{
			m_iCoin++;
		}
		else if (m_iCoin > m_iCopyCoin)
		{
			m_iCoin--;
		}
		else
			m_bAddCoin = false;
	}
		
}

void CUI_Coin::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderObject(CRenderer::RG_UI, this);
}

HRESULT CUI_Coin::Render()
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

	wstring m_strCoin = to_wstring(m_iCoin);
	const _tchar* szCoin = m_strCoin.c_str();

	m_pGameInstance->Draw_Font(TEXT("Font_Nexon"), szCoin , _float2(1500.f, 25.f));

	return S_OK;
}

HRESULT CUI_Coin::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Item_Coin"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;
}

CUI_Coin* CUI_Coin::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_Coin* pInstatnce = new CUI_Coin(pDevice, pContext);

	if (FAILED(pInstatnce->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CUI_Coin"));
		Safe_Release(pInstatnce);
	}

	return pInstatnce;
}

CGameObject* CUI_Coin::Clone(void* pArg)
{
	CUI_Coin* pInstatnce = new CUI_Coin(*this);

	if (FAILED(pInstatnce->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloend : CUI_Coin"));
		Safe_Release(pInstatnce);
	}

	return pInstatnce;
}

void CUI_Coin::Free()
{
	__super::Free();
}
