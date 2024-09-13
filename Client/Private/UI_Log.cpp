#include "stdafx.h"

#include "UI_Log.h"


CUI_Log::CUI_Log(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CUIObject{ pDevice ,pContext }
{
}

CUI_Log::CUI_Log(const CUI_Log& Prototype)
	:CUIObject{ Prototype }
{
}

HRESULT CUI_Log::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Log::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	__super::Set_UI_Setting(800.f, 100.f, g_iWinSizeX * 0.5f, g_iWinSizeY * 0.5f + 250.f);

	m_strWord = TEXT("스테이지에 몬스터들이 많아지고 있어. \n   몬스터들을 제거해줘 보상은 두둑하게 챙겨줄게");

	return S_OK;
}

void CUI_Log::Priority_Update(_float fTimeDelta)
{
	m_fSpeed += fTimeDelta;
	m_fDeleteTimer += fTimeDelta;
}

void CUI_Log::Update(_float fTimeDelta)
{
	_uint iWordLength = m_strWord.size();
	m_strCopyWord.resize(iWordLength);
	if (m_fSpeed >= 0.05f)
	{
		if (iWordLength > m_iIndex)
		{
			m_strCopyWord.at(m_iIndex) = m_strWord.at(m_iIndex);
			m_iIndex++;
		}

		m_fSpeed = 0.f;
	}

	if (m_fDeleteTimer > 0.05f * (iWordLength + 30))
	{
		m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_Quest"), TEXT("Layer_UI_Quest"));
		m_bDead = true;
	}
}

void CUI_Log::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderObject(CRenderer::RG_UI, this);
}

HRESULT CUI_Log::Render()
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

	m_pGameInstance->Draw_Font(TEXT("Font_Nexon"), m_strCopyWord.c_str(), _float2(g_iWinSizeX * 0.5f - 350.f, g_iWinSizeY * 0.5f+ 225), { 0.6235, 0.5725, 0.4353, 1.f }, 0.f, _float2(0.f, 0.f), 0.5f);

	return S_OK;
}

HRESULT CUI_Log::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_Log"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;
}

CUI_Log* CUI_Log::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_Log* pInstatnce = new CUI_Log(pDevice, pContext);

	if (FAILED(pInstatnce->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CUI_Log"));
		Safe_Release(pInstatnce);
	}

	return pInstatnce;
}

CGameObject* CUI_Log::Clone(void* pArg)
{
	CUI_Log* pInstatnce = new CUI_Log(*this);

	if (FAILED(pInstatnce->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloend : CUI_Log"));
		Safe_Release(pInstatnce);
	}

	return pInstatnce;
}

void CUI_Log::Free()
{
	__super::Free();
}
