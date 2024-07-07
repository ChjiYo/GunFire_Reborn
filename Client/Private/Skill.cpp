#include "stdafx.h"

#include "Skill.h"

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

void CSkill::Free()
{
	__super::Free();
}
