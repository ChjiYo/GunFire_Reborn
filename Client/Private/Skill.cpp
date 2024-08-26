#include "stdafx.h"

#include "Skill.h"
#include "Navigation.h"

CSkill::CSkill(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CGameObject{ pDevice ,pContext }
{
}

CSkill::CSkill(const CSkill& Prototype)
	:CGameObject{ Prototype }
{
}

HRESULT CSkill::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CSkill::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CSkill::Priority_Update(_float fTimeDelta)
{
}

void CSkill::Update(_float fTimeDelta)
{
}

void CSkill::Late_Update(_float fTimeDelta)
{
}

HRESULT CSkill::Render()
{
	return S_OK;
}

HRESULT CSkill::Ready_Components()
{
	CNavigation::NAVIGATION_DESC		NavigationDesc{};
	NavigationDesc.iCurrentCellIndex = 0;

	/* Com_Navigation */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Navigation"),
		TEXT("Com_Navigation"), reinterpret_cast<CComponent**>(&m_pNavigationCom), &NavigationDesc)))
		return E_FAIL;

	return S_OK;
}

void CSkill::Free()
{
	__super::Free();
}
