#include "stdafx.h"

#include "UI_Cross_Hair.h"

CUI_Cross_Hair::CUI_Cross_Hair(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CUIObject{ pDevice ,pContext }
{
}

CUI_Cross_Hair::CUI_Cross_Hair(const CUI_Cross_Hair& Prototype)
	:CUIObject{ Prototype }

{
}

HRESULT CUI_Cross_Hair::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Cross_Hair::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	CROSS_HAIR_DESC* Desc =  static_cast<CROSS_HAIR_DESC*>(pArg);
	int iCnt = Desc->iNum;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	//640  360
	switch (iCnt)
	{
	case 0:
		//Right
		__super::Set_UI_Setting(24.f, 7.f, 664.f, 360.f);
		m_pTransformCom->Rotation({ 0.f, 0.f, 1.f }, XMConvertToRadians(0.f));
		break;

	case 1:
		//Left
		__super::Set_UI_Setting(24.f, 7.f, 616.f, 360.f);
		m_pTransformCom->Rotation({ 0.f, 0.f, 1.f }, XMConvertToRadians(180.f));
		break;

	case 2:
		//Up
		__super::Set_UI_Setting(24.f, 7.f, 640.f, 384.f);
		m_pTransformCom->Rotation({ 0.f, 0.f, 1.f }, XMConvertToRadians(270.f));
		break;

	case 3:
		//Down
		__super::Set_UI_Setting(24.f, 7.f, 640.f, 336.f);
		m_pTransformCom->Rotation({ 0.f, 0.f, 1.f }, XMConvertToRadians(90.f));
		break;
	}
	return S_OK;
}

void CUI_Cross_Hair::Priority_Update(_float fTimeDelta)
{

}

void CUI_Cross_Hair::Update(_float fTimeDelta)
{
}

void CUI_Cross_Hair::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderObject(CRenderer::RG_UI, this);
}

HRESULT CUI_Cross_Hair::Render()
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

HRESULT CUI_Cross_Hair::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_Cross_Hair"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;
}

void CUI_Cross_Hair::Move()
{
}

CUI_Cross_Hair* CUI_Cross_Hair::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_Cross_Hair* pInstatnce = new CUI_Cross_Hair(pDevice, pContext);

	if (FAILED(pInstatnce->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CUI_Cross_Hair"));
		Safe_Release(pInstatnce);
	}

	return pInstatnce;
}

CGameObject* CUI_Cross_Hair::Clone(void* pArg)
{
	CUI_Cross_Hair* pInstatnce = new CUI_Cross_Hair(*this);

	if (FAILED(pInstatnce->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloend : CUI_Cross_Hair"));
		Safe_Release(pInstatnce);
	}

	return pInstatnce;
}

void CUI_Cross_Hair::Free()
{
	__super::Free();
}
