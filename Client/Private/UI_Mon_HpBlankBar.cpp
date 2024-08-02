#include "stdafx.h"

#include "UI_Mon_HpBlankBar.h"

CUI_Mon_HpBlankBar::CUI_Mon_HpBlankBar(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CUIObject{ pDevice ,pContext }
{
}

CUI_Mon_HpBlankBar::CUI_Mon_HpBlankBar(const CUI_Mon_HpBlankBar& Prototype)
	:CUIObject{ Prototype }
{
}

HRESULT CUI_Mon_HpBlankBar::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Mon_HpBlankBar::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	__super::Set_UI_Setting(200.f, 30.f, 290.f, 810.f);

	m_pTransformCom->Rotation({ 0.f, 0.f, 1.f }, XMConvertToRadians(355.f));

	return S_OK;
}

void CUI_Mon_HpBlankBar::Priority_Update(_float fTimeDelta)
{
}

void CUI_Mon_HpBlankBar::Update(_float fTimeDelta)
{
}

void CUI_Mon_HpBlankBar::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderObject(CRenderer::RG_UI, this);
}

HRESULT CUI_Mon_HpBlankBar::Render()
{
	if (FAILED(__super::Bind_ShaderResources()))
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

HRESULT CUI_Mon_HpBlankBar::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_Player_HpBlankBar"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;
}

CUI_Mon_HpBlankBar* CUI_Mon_HpBlankBar::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_Mon_HpBlankBar* pInstatnce = new CUI_Mon_HpBlankBar(pDevice, pContext);

	if (FAILED(pInstatnce->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CUI_Mon_HpBlankBar"));
		Safe_Release(pInstatnce);
	}

	return pInstatnce;
}

CGameObject* CUI_Mon_HpBlankBar::Clone(void* pArg)
{
	CUI_Mon_HpBlankBar* pInstatnce = new CUI_Mon_HpBlankBar(*this);

	if (FAILED(pInstatnce->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloend : CUI_Mon_HpBlankBar"));
		Safe_Release(pInstatnce);
	}

	return pInstatnce;
}

void CUI_Mon_HpBlankBar::Free()
{
	__super::Free();
}
