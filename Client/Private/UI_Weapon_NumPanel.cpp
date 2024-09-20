#include "stdafx.h"

#include "UI_Weapon_NumPanel.h"

#include "Player.h"
#include "GameInstance.h"

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

	UI_DESC* Desc = static_cast<UI_DESC*>(pArg);
	m_iCloneIndex = Desc->iNumUI;

	switch (m_iCloneIndex)
	{
	case 0:
		__super::Set_UI_Setting(154.f, 70.f, 1300.f + m_iCloneIndex * 100.f, 850.f);
		break;

	case 1:
		__super::Set_UI_Setting(154.f, 70.f, 1300.f + m_iCloneIndex * 100.f, 850.f);
		break;

	case 2:
		__super::Set_UI_Setting(154.f, 70.f, 1300.f + m_iCloneIndex * 100.f, 850.f);
		break;

	}

	return S_OK;
}

void CUI_Weapon_NumPanel::Priority_Update(_float fTimeDelta)
{
	m_strNum = to_wstring(m_iCloneIndex + 1);
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

	CPlayer* pPlayer = dynamic_cast<CPlayer*>(m_pGameInstance->Get_Layer(m_pGameInstance->GetCurrentLevelIndex(), TEXT("Layer_Player")).back());
	if (pPlayer->Get_Slot() == m_iCloneIndex)
	{
		if (FAILED(m_pShaderCom->Begin(0)))
			return E_FAIL;
	}
	else
	{
		if (FAILED(m_pShaderCom->Begin(5)))
			return E_FAIL;
	}

	if (FAILED(m_pVIBufferCom->Bind_Buffers()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	wstring m_strNum = to_wstring(m_iCloneIndex + 1);
	const _tchar* szNum = m_strNum.c_str();
	
	m_pGameInstance->Draw_Font(TEXT("Font_Nexon"), szNum, _float2(1290.f + m_iCloneIndex * 100.f, 830.f), { 1.f, 1.f , 1.f , 1.f }, 0.f, { 0.f, 0.f }, 1.f);

	return S_OK;
}

HRESULT CUI_Weapon_NumPanel::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Weapon_NumPanel"),
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
