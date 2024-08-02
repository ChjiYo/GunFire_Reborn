#include "stdafx.h"

#include "UI_Mon_BarPanel.h"

CUI_Mon_BarPanel::CUI_Mon_BarPanel(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CUIObject{ pDevice ,pContext }
{
}

CUI_Mon_BarPanel::CUI_Mon_BarPanel(const CUI_Mon_BarPanel& Prototype)
	:CUIObject{ Prototype }
{
}

HRESULT CUI_Mon_BarPanel::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Mon_BarPanel::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	UI_DESC* Desc = static_cast<UI_DESC*>(pArg);
	m_pTargetTransform = Desc->pTargetTransform;
	Safe_AddRef(m_pTargetTransform);

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_pTransformCom->Set_Scaled(2.f , 0.5f, 1.f);
	return S_OK;
}

void CUI_Mon_BarPanel::Priority_Update(_float fTimeDelta)
{
	CTransform* pCamTransform = dynamic_cast<CTransform*>(m_pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_Camera"), TEXT("Com_Transform")));
	m_pTransformCom->LookAt(XMVectorSetY(pCamTransform->Get_State(CTransform::STATE_POSITION), 3.f));
}

void CUI_Mon_BarPanel::Update(_float fTimeDelta)
{
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSetY(m_pTargetTransform->Get_State(CTransform::STATE_POSITION) , 3.f));
}

void CUI_Mon_BarPanel::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderObject(CRenderer::RG_UI, this);
}

HRESULT CUI_Mon_BarPanel::Render()
{
	if (FAILED(__super::Bind_Perspective()))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", 0)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Begin(1)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Bind_Buffers()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Mon_BarPanel::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_Monster_BarPanel"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;
}

CUI_Mon_BarPanel* CUI_Mon_BarPanel::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_Mon_BarPanel* pInstatnce = new CUI_Mon_BarPanel(pDevice, pContext);

	if (FAILED(pInstatnce->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CUI_Mon_BarPanel"));
		Safe_Release(pInstatnce);
	}

	return pInstatnce;
}

CGameObject* CUI_Mon_BarPanel::Clone(void* pArg)
{
	CUI_Mon_BarPanel* pInstatnce = new CUI_Mon_BarPanel(*this);

	if (FAILED(pInstatnce->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloend : CUI_Mon_BarPanel"));
		Safe_Release(pInstatnce);
	}

	return pInstatnce;
}

void CUI_Mon_BarPanel::Free()
{
	Safe_Release(m_pTargetTransform);

	__super::Free();
}
