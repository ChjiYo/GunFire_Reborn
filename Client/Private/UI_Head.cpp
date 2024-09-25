#include "stdafx.h"

#include "UI_Head.h"

#include "Player.h"
#include "GameInstance.h"

#include "Weapon_Player.h"

CUI_Head::CUI_Head(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CUIObject{ pDevice ,pContext }
{
}

CUI_Head::CUI_Head(const CUI_Head& Prototype)
	:CUIObject{ Prototype }

{
}

HRESULT CUI_Head::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Head::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	UI_DESC* Desc = static_cast<UI_DESC*>(pArg);
	_uint iCnt = Desc->iNumUI;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_fMidPosX = Client::g_iWinSizeX * 0.5f;
	m_fMidPosY = Client::g_iWinSizeY * 0.5f;

	m_fAddPos = 36.f;
	//640  360
	switch (iCnt)
	{
	case 0:
		//LU
		__super::Set_UI_Setting(64.f, 32.f, m_fMidPosX - m_fAddPos, m_fMidPosY - m_fAddPos);
		m_pTransformCom->Rotation({ 0.f, 0.f, 1.f }, XMConvertToRadians(315.f));
		break;

	case 1:
		//RU
		__super::Set_UI_Setting(64.f, 32.f, m_fMidPosX + m_fAddPos, m_fMidPosY - m_fAddPos);
		m_pTransformCom->Rotation({ 0.f, 0.f, 1.f }, XMConvertToRadians(225.f));
		break;

	case 2:
		//LD
		__super::Set_UI_Setting(64.f, 32.f, m_fMidPosX - m_fAddPos, m_fMidPosY + m_fAddPos);
		m_pTransformCom->Rotation({ 0.f, 0.f, 1.f }, XMConvertToRadians(45.f));
		break;

	case 3:
		//RD
		__super::Set_UI_Setting(64.f, 32.f, m_fMidPosX + m_fAddPos, m_fMidPosY + m_fAddPos);
		m_pTransformCom->Rotation({ 0.f, 0.f, 1.f }, XMConvertToRadians(135.f));
		break;
	}
	return S_OK;
}

void CUI_Head::Priority_Update(_float fTimeDelta)
{
	m_fTimer += fTimeDelta;

	if (m_fTimer >= 1.f)
	{
		m_fTimer = 0.f;
		m_bDead = true;
	}
}

void CUI_Head::Update(_float fTimeDelta)
{
}

void CUI_Head::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderObject(CRenderer::RG_UI, this);
}

HRESULT CUI_Head::Render()
{
	if (FAILED(__super::Bind_ShaderResources()))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_Timer", &m_fTimer, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", 0)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Begin(7)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Bind_Buffers()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Head::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Head"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;
}

CUI_Head* CUI_Head::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_Head* pInstance = new CUI_Head(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CUI_Head"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_Head::Clone(void* pArg)
{
	CUI_Head* pInstance = new CUI_Head(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloend : CUI_Head"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Head::Free()
{
	__super::Free();
}
