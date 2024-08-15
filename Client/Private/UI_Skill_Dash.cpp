#include "stdafx.h"

#include "UI_Skill_Dash.h"

CUI_Skill_Dash::CUI_Skill_Dash(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CUIObject{ pDevice ,pContext }
{
}

CUI_Skill_Dash::CUI_Skill_Dash(const CUI_Skill_Dash& Prototype)
	:CUIObject{ Prototype }
{
}

HRESULT CUI_Skill_Dash::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Skill_Dash::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	__super::Set_UI_Setting(35.f, 35.f, 1400.f, 750.f);

	return S_OK;
}

void CUI_Skill_Dash::Priority_Update(_float fTimeDelta)
{
}

void CUI_Skill_Dash::Update(_float fTimeDelta)
{
}

void CUI_Skill_Dash::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderObject(CRenderer::RG_UI, this);
}

HRESULT CUI_Skill_Dash::Render()
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

HRESULT CUI_Skill_Dash::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_Weapon_YellowBullet"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;
}

CUI_Skill_Dash* CUI_Skill_Dash::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_Skill_Dash* pInstatnce = new CUI_Skill_Dash(pDevice, pContext);

	if (FAILED(pInstatnce->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CUI_Skill_Dash"));
		Safe_Release(pInstatnce);
	}

	return pInstatnce;
}

CGameObject* CUI_Skill_Dash::Clone(void* pArg)
{
	CUI_Skill_Dash* pInstatnce = new CUI_Skill_Dash(*this);

	if (FAILED(pInstatnce->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloend : CUI_Skill_Dash"));
		Safe_Release(pInstatnce);
	}

	return pInstatnce;
}

void CUI_Skill_Dash::Free()
{
	__super::Free();
}
