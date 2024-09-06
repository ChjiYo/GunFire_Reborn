#include "stdafx.h"

#include "UI_Boss_HpBar.h"

CUI_Boss_HpBar::CUI_Boss_HpBar(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CUIObject{ pDevice ,pContext }
{
}

CUI_Boss_HpBar::CUI_Boss_HpBar(const CUI_Boss_HpBar& Prototype)
	:CUIObject{ Prototype }
{
}

HRESULT CUI_Boss_HpBar::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Boss_HpBar::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	__super::Set_UI_Setting(450.f, 10.f, g_iWinSizeX * 0.5f, 60.f);

	UI_DESC* pDesc = static_cast<UI_DESC*>(pArg);
	m_pBoss = dynamic_cast<CMonster*>(pDesc->pGameObject);

	return S_OK;

}

void CUI_Boss_HpBar::Priority_Update(_float fTimeDelta)
{
	m_strHp = to_wstring(m_pBoss->Get_MonDesc().iHp);
	m_strMaxHp = to_wstring(m_pBoss->Get_MonDesc().iMaxHp);
	
	m_fHpRatio = static_cast<_float>(m_pBoss->Get_MonDesc().iHp) / static_cast<_float>(m_pBoss->Get_MonDesc().iMaxHp);
}

void CUI_Boss_HpBar::Update(_float fTimeDelta)
{
}

void CUI_Boss_HpBar::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderObject(CRenderer::RG_UI, this);
}

HRESULT CUI_Boss_HpBar::Render()
{
	if (FAILED(__super::Bind_ShaderResources()))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_Ratio", &m_fHpRatio, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", 0)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Begin(3)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Bind_Buffers()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	

	return S_OK;
}

HRESULT CUI_Boss_HpBar::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_BOSS, TEXT("Prototype_Component_Texture_UI_Boss_HpBar"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;
}

CUI_Boss_HpBar* CUI_Boss_HpBar::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_Boss_HpBar* pInstatnce = new CUI_Boss_HpBar(pDevice, pContext);

	if (FAILED(pInstatnce->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CUI_Boss_HpBar"));
		Safe_Release(pInstatnce);
	}

	return pInstatnce;
}

CGameObject* CUI_Boss_HpBar::Clone(void* pArg)
{
	CUI_Boss_HpBar* pInstatnce = new CUI_Boss_HpBar(*this);

	if (FAILED(pInstatnce->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloend : CUI_Boss_HpBar"));
		Safe_Release(pInstatnce);
	}

	return pInstatnce;
}

void CUI_Boss_HpBar::Free()
{
	//Safe_Release(m_pBoss);

	__super::Free();
}
