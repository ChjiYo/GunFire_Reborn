#include "stdafx.h"

#include "UI_Quest.h"


CUI_Quest::CUI_Quest(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CUIObject{ pDevice ,pContext }
{
}

CUI_Quest::CUI_Quest(const CUI_Quest& Prototype)
	:CUIObject{ Prototype }
{
}

HRESULT CUI_Quest::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Quest::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_fSizeX = 40.f;
	m_fSizeY = 10.f;
	m_fPosX = 200;
	m_fPosY = 700;

	__super::Set_UI_Setting(m_fSizeX, m_fSizeY, m_fPosX, m_fPosY);
  
	return S_OK;
}

void CUI_Quest::Priority_Update(_float fTimeDelta)
{
	m_fScaleWeight += fTimeDelta * 5.f;

}

void CUI_Quest::Update(_float fTimeDelta)
{
	if (10.f >= m_fScaleWeight)
	{
		m_fSizeX = m_fScaleWeight * 40;
		m_fSizeY = m_fScaleWeight * 10;

		m_pTransformCom->Set_Scaled(m_fSizeX, m_fSizeY, 1.f);
	}
	else
		m_bFont = true;
}

void CUI_Quest::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderObject(CRenderer::RG_UI, this);
}

HRESULT CUI_Quest::Render()
{
	if (FAILED(__super::Bind_ShaderResources()))
		return E_FAIL;;

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", 0)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Begin(5)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Bind_Buffers()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	if(m_bFont)
		m_pGameInstance->Draw_Font(TEXT("Font_Nexon"), TEXT(" 스테이지에 있는 몬스터들 \n 제거하기 [ ] "), _float2(50.f, 675.f), {0.6235, 0.5725, 0.4353, 1.f}, 0.f, _float2(0.f, 0.f), 0.5f);

	return S_OK;
}

HRESULT CUI_Quest::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_Quest"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;
}

CUI_Quest* CUI_Quest::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_Quest* pInstatnce = new CUI_Quest(pDevice, pContext);

	if (FAILED(pInstatnce->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CUI_Quest"));
		Safe_Release(pInstatnce);
	}

	return pInstatnce;
}

CGameObject* CUI_Quest::Clone(void* pArg)
{
	CUI_Quest* pInstatnce = new CUI_Quest(*this);

	if (FAILED(pInstatnce->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloend : CUI_Quest"));
		Safe_Release(pInstatnce);
	}

	return pInstatnce;
}

void CUI_Quest::Free()
{
	__super::Free();
}
