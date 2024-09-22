#include "stdafx.h"

#include "UI_MiniMap.h"


CUI_MiniMap::CUI_MiniMap(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CUIObject{ pDevice ,pContext }
{
}

CUI_MiniMap::CUI_MiniMap(const CUI_MiniMap& Prototype)
	:CUIObject{ Prototype }
{
}

HRESULT CUI_MiniMap::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_MiniMap::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	__super::Set_UI_Setting(200.f, 200.f, 150.f, 150.f);

	return S_OK;
}

void CUI_MiniMap::Priority_Update(_float fTimeDelta)
{

}

void CUI_MiniMap::Update(_float fTimeDelta)
{
	list<CGameObject*> pMonList1 =  m_pGameInstance->Get_Layer(m_pGameInstance->GetCurrentLevelIndex(), TEXT("Layer_Monster_Stage1"));
	list<CGameObject*> pMonList2 = m_pGameInstance->Get_Layer(m_pGameInstance->GetCurrentLevelIndex(), TEXT("Layer_Monster_Stage2"));

	CTransform* pPlayerTransform = dynamic_cast<CTransform*>(m_pGameInstance->Get_Component(m_pGameInstance->GetCurrentLevelIndex(), TEXT("Layer_Player"), TEXT("Com_Transform")));
	_vector vPlayerPos = pPlayerTransform->Get_State(CTransform::STATE_POSITION);
		

	for (auto& iter : pMonList1)
	{
		_vector vMonPos = iter->Get_Transform()->Get_State(CTransform::STATE_POSITION);

		_float fDist = XMVectorGetX(XMVector4Length(vMonPos - vPlayerPos));

		if (fDist <= 30.f)
		{
			if (!dynamic_cast<CMonster*>(iter)->Get_Mark())
			{
				CUIObject::UI_DESC Desc = {};
				Desc.pTransform = iter->Get_Transform();
				Desc.pGameObject = iter;

				m_pGameInstance->Add_GameObject_ToLayer(m_pGameInstance->GetCurrentLevelIndex(), TEXT("Prototype_GameObject_UI_Mark"), TEXT("Layer_Mark"),&Desc);
				dynamic_cast<CMonster*>(iter)->Set_Mark(true);
			}
		}
		else
			dynamic_cast<CMonster*>(iter)->Set_Mark(false);
	}
	
}

void CUI_MiniMap::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderObject(CRenderer::RG_UI, this);
}

HRESULT CUI_MiniMap::Render()
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

HRESULT CUI_MiniMap::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_MiniMap"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;
}

CUI_MiniMap* CUI_MiniMap::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_MiniMap* pInstatnce = new CUI_MiniMap(pDevice, pContext);

	if (FAILED(pInstatnce->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CUI_MiniMap"));
		Safe_Release(pInstatnce);
	}

	return pInstatnce;
}

CGameObject* CUI_MiniMap::Clone(void* pArg)
{
	CUI_MiniMap* pInstatnce = new CUI_MiniMap(*this);

	if (FAILED(pInstatnce->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloend : CUI_MiniMap"));
		Safe_Release(pInstatnce);
	}

	return pInstatnce;
}

void CUI_MiniMap::Free()
{
	__super::Free();
}
