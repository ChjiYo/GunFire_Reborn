#include "stdafx.h"

#include "UI_Sniper_Ring.h"

CUI_Sniper_Ring::CUI_Sniper_Ring(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CUIObject{ pDevice ,pContext }
{
}

CUI_Sniper_Ring::CUI_Sniper_Ring(const CUI_Sniper_Ring& Prototype)
	:CUIObject{ Prototype }

{
}

HRESULT CUI_Sniper_Ring::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Sniper_Ring::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	__super::Set_UI_Setting(900.f, 900.f, g_iWinSizeX * 0.5f, g_iWinSizeY * 0.5f);

	return S_OK;
}

void CUI_Sniper_Ring::Priority_Update(_float fTimeDelta)
{
	
}

void CUI_Sniper_Ring::Update(_float fTimeDelta)
{
}

void CUI_Sniper_Ring::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderObject(CRenderer::RG_UI, this);
}

HRESULT CUI_Sniper_Ring::Render()
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

HRESULT CUI_Sniper_Ring::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Sniper_Ring"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;
}

CUI_Sniper_Ring* CUI_Sniper_Ring::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_Sniper_Ring* pInstance = new CUI_Sniper_Ring(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CUI_Sniper_Ring"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_Sniper_Ring::Clone(void* pArg)
{
	CUI_Sniper_Ring* pInstance = new CUI_Sniper_Ring(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloend : CUI_Sniper_Ring"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Sniper_Ring::Free()
{
	__super::Free();
}
