#include "stdafx.h"
#include "..\Public\Monster.h"

#include "GameInstance.h"

CMonster::CMonster(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CContainerObject{ pDevice, pContext }
{

}

CMonster::CMonster(const CMonster & Prototype)
	: CContainerObject{ Prototype }
{

}

HRESULT CMonster::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CMonster::Initialize(void * pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CMonster::Priority_Update(_float fTimeDelta)
{
		
}

void CMonster::Update(_float fTimeDelta)
{

}

void CMonster::Late_Update(_float fTimeDelta)
{
	m_pTransformCom->Gravity(0.2f, 0.25f, fTimeDelta);
}

HRESULT CMonster::Render()
{
	return S_OK;
}

HRESULT CMonster::Ready_Components()
{
	CNavigation::NAVIGATION_DESC		NavigationDesc{};
	NavigationDesc.iCurrentCellIndex = 0;

	/* Com_Navigation */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Navigation"),
		TEXT("Com_Navigation"), reinterpret_cast<CComponent**>(&m_pNavigationCom), &NavigationDesc)))
		return E_FAIL;

	
	return S_OK;
}

HRESULT CMonster::Ready_UI()
{
	CUIObject::UI_DESC Desc = {};
	Desc.pTransform = m_pTransformCom;
	Desc.pGameObject = this;

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_Monster_BarPanel"), TEXT("Layer_UI_Monster_ABarPanel"), &Desc)))
		return E_FAIL;
	
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_Monster_HpBar"), TEXT("Layer_UI_Monster_CHpBar"),&Desc)))
		return E_FAIL;
	
	return S_OK;
}

void CMonster::Free()
{
	__super::Free();
}
