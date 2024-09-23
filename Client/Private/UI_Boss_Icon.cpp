#include "stdafx.h"

#include "UI_Boss_Icon.h"

#include "GameInstance.h"
#include "Mon_Dragon.h"

CUI_Boss_Icon::CUI_Boss_Icon(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CUIObject{ pDevice ,pContext }
{
}

CUI_Boss_Icon::CUI_Boss_Icon(const CUI_Boss_Icon& Prototype)
	:CUIObject{ Prototype }
{
}

HRESULT CUI_Boss_Icon::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Boss_Icon::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	__super::Set_UI_Setting( 100.f, 100.f, g_iWinSizeX * 0.5f - 550.f *0.5f , 60);

	return S_OK;
}

void CUI_Boss_Icon::Priority_Update(_float fTimeDelta)
{
}

void CUI_Boss_Icon::Update(_float fTimeDelta)
{
	CMon_Dragon* pBoss = dynamic_cast<CMon_Dragon*>(m_pGameInstance->Get_Layer(LEVEL_BOSS, TEXT("Layer_Monster")).back());

	if (pBoss->Get_MonDesc().iHp <= 0)
		m_bDead = true;
}

void CUI_Boss_Icon::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderObject(CRenderer::RG_UI, this);
}

HRESULT CUI_Boss_Icon::Render()
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

HRESULT CUI_Boss_Icon::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_BOSS, TEXT("Prototype_Component_Texture_UI_Boss_Icon"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;
}

CUI_Boss_Icon* CUI_Boss_Icon::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_Boss_Icon* pInstatnce = new CUI_Boss_Icon(pDevice, pContext);

	if (FAILED(pInstatnce->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CUI_Boss_Icon"));
		Safe_Release(pInstatnce);
	}

	return pInstatnce;
}

CGameObject* CUI_Boss_Icon::Clone(void* pArg)
{
	CUI_Boss_Icon* pInstatnce = new CUI_Boss_Icon(*this);

	if (FAILED(pInstatnce->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloend : CUI_Boss_Icon"));
		Safe_Release(pInstatnce);
	}

	return pInstatnce;
}

void CUI_Boss_Icon::Free()
{
	__super::Free();
}
