#include "stdafx.h"

#include "UI_Sniper_Hair.h"

CUI_Sniper_Hair::CUI_Sniper_Hair(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CUIObject{ pDevice ,pContext }
{
}

CUI_Sniper_Hair::CUI_Sniper_Hair(const CUI_Sniper_Hair& Prototype)
	:CUIObject{ Prototype }

{
}

HRESULT CUI_Sniper_Hair::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Sniper_Hair::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;


	SNIPER_HAIR_DESC* Desc = static_cast<SNIPER_HAIR_DESC*>(pArg);
	_uint iCnt = Desc->iNum;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_fMidPosX = Client::g_iWinSizeX * 0.5f;
	m_fMidPosY = Client::g_iWinSizeY * 0.5f;

	m_fAddPos = 200.f;
	//640  360
	switch (iCnt)
	{
	case 0:
		//Right
		__super::Set_UI_Setting(110.f, 155.f, m_fMidPosX + m_fAddPos, m_fMidPosY + m_fAddPos);
		m_pTransformCom->Rotation({ 0.f, 0.f, 1.f }, XMConvertToRadians(180.f));
		m_iTextureIndex = 0;
		break;

	case 1:
		//Left
		__super::Set_UI_Setting(110.f, 155.f, m_fMidPosX - m_fAddPos, m_fMidPosY - m_fAddPos);
		m_pTransformCom->Rotation({ 0.f, 0.f, 1.f }, XMConvertToRadians(00.f));
		m_iTextureIndex = 0;
		break;

	case 2:
		//Up
		__super::Set_UI_Setting(110.f, 155.f, m_fMidPosX - m_fAddPos, m_fMidPosY + m_fAddPos);
		m_pTransformCom->Rotation({ 0.f, 0.f, 1.f }, XMConvertToRadians(180.f));
		m_iTextureIndex = 1;
		break;

	case 3:
		//Down
		__super::Set_UI_Setting(110.f, 155.f, m_fMidPosX + m_fAddPos, m_fMidPosY - m_fAddPos);
		m_pTransformCom->Rotation({ 0.f, 0.f, 1.f }, XMConvertToRadians(0.f));
		m_iTextureIndex = 1;
		break;
	}
	return S_OK;
}

void CUI_Sniper_Hair::Priority_Update(_float fTimeDelta)
{

}

void CUI_Sniper_Hair::Update(_float fTimeDelta)
{
}

void CUI_Sniper_Hair::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderObject(CRenderer::RG_UI, this);
}

HRESULT CUI_Sniper_Hair::Render()
{
	if (FAILED(__super::Bind_ShaderResources()))
		return E_FAIL;

	/*for (int i = 1; i <= 4; ++i)
	{
		if (FAILED(m_pShaderCom->Bind_RawValue("g_Num", &i, sizeof(_uint))))
			return E_FAIL;
	}*/

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", m_iTextureIndex)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Begin(0)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Bind_Buffers()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Sniper_Hair::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_Sniper_Hair"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;
}

CUI_Sniper_Hair* CUI_Sniper_Hair::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_Sniper_Hair* pInstance = new CUI_Sniper_Hair(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CUI_Sniper_Hair"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_Sniper_Hair::Clone(void* pArg)
{
	CUI_Sniper_Hair* pInstance = new CUI_Sniper_Hair(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloend : CUI_Sniper_Hair"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Sniper_Hair::Free()
{
	__super::Free();
}
