#include "stdafx.h"

#include "UI_Mark.h"


CUI_Mark::CUI_Mark(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CUIObject{ pDevice ,pContext }
{
}

CUI_Mark::CUI_Mark(const CUI_Mark& Prototype)
	:CUIObject{ Prototype }
{
}

HRESULT CUI_Mark::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Mark::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	UI_DESC* pDesc = static_cast<UI_DESC*>(pArg);

	m_pMonTransform = pDesc->pTransform;
	m_pMonster = dynamic_cast<CMonster*>(pDesc->pGameObject);
	Safe_AddRef(m_pMonTransform);
	Safe_AddRef(m_pMonster);

	m_fPosX = 1450.f;
	m_fPosY= 200.f;
	
	return S_OK;
}

void CUI_Mark::Priority_Update(_float fTimeDelta)
{

}

void CUI_Mark::Update(_float fTimeDelta)
{
	CTransform* pPlayerTransform = dynamic_cast<CTransform*>(m_pGameInstance->Get_Component(m_pGameInstance->GetCurrentLevelIndex(), TEXT("Layer_Player"), TEXT("Com_Transform")));

	_vector vPlayerPos = pPlayerTransform->Get_State(CTransform::STATE_POSITION);
	_vector vMonPos = m_pMonTransform->Get_State(CTransform::STATE_POSITION);

	_float fDist = XMVectorGetX(XMVector4Length(vMonPos - vPlayerPos));

	_float fAngle = pPlayerTransform->Target_Look_Degree(vMonPos) + 90.f;

	_float fOffSetX = m_fPosX + (fDist * 2) * cos(fAngle * (3.14f / 180.f));
	_float fOffSetY = m_fPosY - (fDist * 2) * sin(fAngle * (3.14f / 180.f));

	__super::Set_UI_Setting(20.f, 20.f, fOffSetX, fOffSetY);

	if (fDist > 30.f)
		m_bDead = true;

	if(m_pMonster->Get_Dead())
		m_bDead = true;

}

void CUI_Mark::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderObject(CRenderer::RG_UI, this);
}

HRESULT CUI_Mark::Render()
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

	return S_OK;
}

HRESULT CUI_Mark::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Mark"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;
}

CUI_Mark* CUI_Mark::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_Mark* pInstatnce = new CUI_Mark(pDevice, pContext);

	if (FAILED(pInstatnce->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CUI_Mark"));
		Safe_Release(pInstatnce);
	}

	return pInstatnce;
}

CGameObject* CUI_Mark::Clone(void* pArg)
{
	CUI_Mark* pInstatnce = new CUI_Mark(*this);

	if (FAILED(pInstatnce->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloend : CUI_Mark"));
		Safe_Release(pInstatnce);
	}

	return pInstatnce;
}

void CUI_Mark::Free()
{
	Safe_Release(m_pMonTransform);
	Safe_Release(m_pMonster);

	__super::Free();
}
