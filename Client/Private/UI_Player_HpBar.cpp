#include "stdafx.h"

#include "UI_Player_HpBar.h"

CUI_Player_HpBar::CUI_Player_HpBar(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CUIObject{ pDevice ,pContext }
{
}

CUI_Player_HpBar::CUI_Player_HpBar(const CUI_Player_HpBar& Prototype)
	:CUIObject{ Prototype }
{
}

HRESULT CUI_Player_HpBar::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Player_HpBar::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_fSizeX = 251.f;
	m_fSizeY = 11.f;
	m_fPosX = 290.f;
	m_fPosY = 810.f;

	__super::Set_UI_Setting(m_fSizeX, m_fSizeY, m_fPosX, m_fPosY);
	m_pTransformCom->Rotation({ 0.f , 0.f, 1.f }, XMConvertToRadians(5.f));

	m_pPlayer = dynamic_cast<CPlayer*>(m_pGameInstance->Get_Layer(m_pGameInstance->GetLoadingLevelIndex(), TEXT("Layer_Player")).back());
	Safe_AddRef(m_pPlayer);
	
	return S_OK;

}

void CUI_Player_HpBar::Priority_Update(_float fTimeDelta)
{
	m_strHp = to_wstring(m_pPlayer->Get_Player_Desc().iHp);
	m_strMaxHp = to_wstring(m_pPlayer->Get_Player_Desc().iMaxHp);

	m_fHpRatio = static_cast<_float>(m_pPlayer->Get_Player_Desc().iHp) / static_cast<_float>(m_pPlayer->Get_Player_Desc().iMaxHp);
}

void CUI_Player_HpBar::Update(_float fTimeDelta)
{
}

void CUI_Player_HpBar::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderObject(CRenderer::RG_UI, this);
}

HRESULT CUI_Player_HpBar::Render()
{
	if (FAILED(__super::Bind_ShaderResources()))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_Ratio", &m_fHpRatio, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", 0)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Begin(2)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Bind_Buffers()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	m_pGameInstance->Draw_Font(TEXT("Font_Nexon"), m_strHp.c_str(), _float2(m_fPosX + 140, m_fPosY - 23), { 0.611f, 1.f , 0.329f , 1.f }, 0.f, {0.f, 0.f},0.75);
	m_pGameInstance->Draw_Font(TEXT("Font_Nexon"), TEXT(" /"), _float2(m_fPosX + 166, m_fPosY - 23), {0.611f, 1.f , 0.329f , 1.f}, 0.f, {0.f, 0.f}, 0.75);
	m_pGameInstance->Draw_Font(TEXT("Font_Nexon"), m_strMaxHp.c_str(), _float2(m_fPosX + 190 , m_fPosY - 23), {0.611f, 1.f , 0.329f , 1.f}, 0.f, {0.f, 0.f}, 0.75);
	
	return S_OK;
}

HRESULT CUI_Player_HpBar::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Player_HpBar"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;
}

CUI_Player_HpBar* CUI_Player_HpBar::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_Player_HpBar* pInstatnce = new CUI_Player_HpBar(pDevice, pContext);

	if (FAILED(pInstatnce->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CUI_Player_HpBar"));
		Safe_Release(pInstatnce);
	}

	return pInstatnce;
}

CGameObject* CUI_Player_HpBar::Clone(void* pArg)
{
	CUI_Player_HpBar* pInstatnce = new CUI_Player_HpBar(*this);

	if (FAILED(pInstatnce->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloend : CUI_Player_HpBar"));
		Safe_Release(pInstatnce);
	}

	return pInstatnce;
}

void CUI_Player_HpBar::Free()
{
	Safe_Release(m_pPlayer);

	__super::Free();
}
