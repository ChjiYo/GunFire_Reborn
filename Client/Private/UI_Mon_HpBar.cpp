#include "stdafx.h"

#include "UI_Mon_HpBar.h"
#include "Mon_Archer.h"

CUI_Mon_HpBar::CUI_Mon_HpBar(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CUIObject{ pDevice ,pContext }
{
}

CUI_Mon_HpBar::CUI_Mon_HpBar(const CUI_Mon_HpBar& Prototype)
	:CUIObject{ Prototype }
{
}

HRESULT CUI_Mon_HpBar::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Mon_HpBar::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	UI_DESC* Desc = static_cast<UI_DESC*>(pArg);
	m_pOriginTransform = Desc->pTransform;
	m_pMonster = dynamic_cast<CMonster*>(Desc->pGameObject);

	Safe_AddRef(m_pOriginTransform);
	Safe_AddRef(m_pMonster);

	MON_HP_DESC* pHpDesc = static_cast<MON_HP_DESC*>(pArg);
	m_fOffsetY = pHpDesc->fOffsetY;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_fSizeX = 1.5f;
	m_fPosX = XMVectorGetX(m_pTransformCom->Get_State(CTransform::STATE_POSITION));
	m_pTransformCom->Set_Scaled(1.f, 0.1f, 1.f);
	
	return S_OK;
}

void CUI_Mon_HpBar::Priority_Update(_float fTimeDelta)
{
	CTransform* pCamTransform = dynamic_cast<CTransform*>(m_pGameInstance->Get_Component(m_pGameInstance->GetLoadingLevelIndex(), TEXT("Layer_Camera"), TEXT("Com_Transform")));

	_float3 vPos = {};
	XMStoreFloat3(&vPos , pCamTransform->Get_State(CTransform::STATE_POSITION));

	_vector vTmp = { vPos.x ,3.f,  vPos.z , 1.f };
	m_pTransformCom->LookAt(vTmp);
}

void CUI_Mon_HpBar::Update(_float fTimeDelta)
{
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSetY(m_pOriginTransform->Get_State(CTransform::STATE_POSITION), XMVectorGetY(m_pOriginTransform->Get_State(CTransform::STATE_POSITION))+ m_fOffsetY));

	fHpRatio = static_cast<float>(m_pMonster->Get_MonDesc().iHp) / static_cast<float>(m_pMonster->Get_MonDesc().iMaxHp);

	Destroy();
}

void CUI_Mon_HpBar::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderObject(CRenderer::RG_UI, this);
}

HRESULT CUI_Mon_HpBar::Render()
{
	if(FAILED(Bind_ShaderResources()))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", 0)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Begin(2)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Bind_Buffers()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;


	return S_OK;
}

void CUI_Mon_HpBar::Destroy()
{
	if (m_pMonster->Get_MonDesc().iHp <= 0 || m_pMonster->Get_Dead())
		m_bDead = true;
}

HRESULT CUI_Mon_HpBar::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_UI_Monster_HpBar"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;
}

HRESULT CUI_Mon_HpBar::Bind_ShaderResources()
{
	if(FAILED(__super::Bind_Perspective()))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_Ratio", &fHpRatio,sizeof(_float))))
		return E_FAIL;

	return S_OK;
}

CUI_Mon_HpBar* CUI_Mon_HpBar::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_Mon_HpBar* pInstatnce = new CUI_Mon_HpBar(pDevice, pContext);

	if (FAILED(pInstatnce->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CUI_Mon_HpBar"));
		Safe_Release(pInstatnce);
	}

	return pInstatnce;
}

CGameObject* CUI_Mon_HpBar::Clone(void* pArg)
{
	CUI_Mon_HpBar* pInstatnce = new CUI_Mon_HpBar(*this);

	if (FAILED(pInstatnce->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloend : CUI_Mon_HpBar"));
		Safe_Release(pInstatnce);
	}

	return pInstatnce;
}

void CUI_Mon_HpBar::Free() 
{
	Safe_Release(m_pOriginTransform);
	Safe_Release(m_pMonster);

	__super::Free();
}
