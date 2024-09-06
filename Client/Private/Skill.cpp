#include "stdafx.h"

#include "Skill.h"
#include "Navigation.h"

CSkill::CSkill(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CContainerObject{ pDevice ,pContext }
{
}

CSkill::CSkill(const CSkill& Prototype)
	:CContainerObject{ Prototype }
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

	//m_iSkillDamage = static_cast<>

	return S_OK;
}

void CSkill::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

void CSkill::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);
}

void CSkill::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CSkill::Render()
{
	return S_OK;
}

HRESULT CSkill::Ready_Components()
{
	return S_OK;
}

void CSkill::Free()
{
	__super::Free();
}
