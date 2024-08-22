#include "stdafx.h"

#include "UI_Player_ShieldBar.h"

CUI_Player_ShieldBar::CUI_Player_ShieldBar(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CUIObject{ pDevice ,pContext }
{
}

CUI_Player_ShieldBar::CUI_Player_ShieldBar(const CUI_Player_ShieldBar& Prototype)
	:CUIObject{ Prototype }
{
}

HRESULT CUI_Player_ShieldBar::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Player_ShieldBar::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_fSizeX = 251;
	m_fSizeY = 11.f;
	m_fPosX = 290.f;
	m_fPosY = 785.f;

	__super::Set_UI_Setting(m_fSizeX, m_fSizeY, m_fPosX, m_fPosY);
	m_pTransformCom->Rotation({ 0.f , 0.f, 1.f }, XMConvertToRadians(5.f));
	
	m_pPlayer = dynamic_cast<CPlayer*>(m_pGameInstance->Get_Layer(LEVEL_GAMEPLAY, TEXT("Layer_Player")).back());
	Safe_AddRef(m_pPlayer);

	return S_OK;
}

void CUI_Player_ShieldBar::Priority_Update(_float fTimeDelta)
{
	m_strShield = to_wstring(m_pPlayer->Get_Player_Desc().iShield);
	m_strMaxShield = to_wstring(m_pPlayer->Get_Player_Desc().iMaxShield);

	m_fShieldRatio = static_cast<_float>(m_pPlayer->Get_Player_Desc().iShield) / static_cast<_float>(m_pPlayer->Get_Player_Desc().iMaxShield);
}

void CUI_Player_ShieldBar::Update(_float fTimeDelta)
{
}

void CUI_Player_ShieldBar::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderObject(CRenderer::RG_UI, this);
}

HRESULT CUI_Player_ShieldBar::Render()
{
	if (FAILED(__super::Bind_ShaderResources()))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_Ratio", &m_fShieldRatio, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", 0)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Begin(2)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Bind_Buffers()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	m_pGameInstance->Draw_Font(TEXT("Font_Nexon"), m_strShield + TEXT(" / ") + m_strMaxShield, _float2(m_fPosX + 100.f, m_fPosY), XMVectorSet(0.1f, 0.9f, 1.f, 1.f), XMConvertToRadians(-5.f), 0.8f);
	
	return S_OK;
}

HRESULT CUI_Player_ShieldBar::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_Player_ShieldBar"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;
}

CUI_Player_ShieldBar* CUI_Player_ShieldBar::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_Player_ShieldBar* pInstatnce = new CUI_Player_ShieldBar(pDevice, pContext);

	if (FAILED(pInstatnce->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CUI_Player_ShieldBar"));
		Safe_Release(pInstatnce);
	}

	return pInstatnce;
}

CGameObject* CUI_Player_ShieldBar::Clone(void* pArg)
{
	CUI_Player_ShieldBar* pInstatnce = new CUI_Player_ShieldBar(*this);

	if (FAILED(pInstatnce->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloend : CUI_Player_ShieldBar"));
		Safe_Release(pInstatnce);
	}

	return pInstatnce;
}

void CUI_Player_ShieldBar::Free()
{
	Safe_Release(m_pPlayer);

	__super::Free();
}
