#include "stdafx.h"

#include "UI_Boss_HpPanel.h"

CUI_Boss_HpPanel::CUI_Boss_HpPanel(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CUIObject{ pDevice ,pContext }
{
}

CUI_Boss_HpPanel::CUI_Boss_HpPanel(const CUI_Boss_HpPanel& Prototype)
	:CUIObject{ Prototype }
{
}

HRESULT CUI_Boss_HpPanel::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Boss_HpPanel::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;
	__super::Set_UI_Setting(550.f, 60.f, g_iWinSizeX * 0.5f, 60);

	//m_pPlayer = dynamic_cast<CPlayer*>(m_pGameInstance->Get_Layer(m_pGameInstance->GetLoadingLevelIndex(), TEXT("Layer_Player")).back());
	//Safe_AddRef(m_pPlayer);

	return S_OK;

}

void CUI_Boss_HpPanel::Priority_Update(_float fTimeDelta)
{
	//m_strHp = to_wstring(m_pPlayer->Get_Player_Desc().iHp);
	//m_strMaxHp = to_wstring(m_pPlayer->Get_Player_Desc().iMaxHp);
	//
	//m_fHpRatio = static_cast<_float>(m_pPlayer->Get_Player_Desc().iHp) / static_cast<_float>(m_pPlayer->Get_Player_Desc().iMaxHp);
}

void CUI_Boss_HpPanel::Update(_float fTimeDelta)
{
}

void CUI_Boss_HpPanel::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderObject(CRenderer::RG_UI, this);
}

HRESULT CUI_Boss_HpPanel::Render()
{
	if (FAILED(__super::Bind_ShaderResources()))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_Ratio", &m_fHpRatio, sizeof(_float))))
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

HRESULT CUI_Boss_HpPanel::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_BOSS, TEXT("Prototype_Component_Texture_UI_Boss_Hp_Panel"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;
}

CUI_Boss_HpPanel* CUI_Boss_HpPanel::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_Boss_HpPanel* pInstatnce = new CUI_Boss_HpPanel(pDevice, pContext);

	if (FAILED(pInstatnce->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CUI_Boss_HpPanel"));
		Safe_Release(pInstatnce);
	}

	return pInstatnce;
}

CGameObject* CUI_Boss_HpPanel::Clone(void* pArg)
{
	CUI_Boss_HpPanel* pInstatnce = new CUI_Boss_HpPanel(*this);

	if (FAILED(pInstatnce->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloend : CUI_Boss_HpPanel"));
		Safe_Release(pInstatnce);
	}

	return pInstatnce;
}

void CUI_Boss_HpPanel::Free()
{
	//Safe_Release(m_pPlayer);

	__super::Free();
}
