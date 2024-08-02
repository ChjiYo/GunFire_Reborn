#include "stdafx.h"

#include "Skill_Monster.h"

CSkill_Monster::CSkill_Monster(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CSkill{ pDevice  , pContext }
{
}

CSkill_Monster::CSkill_Monster(const CSkill& Prototype)
	:CSkill{ Prototype }
{
}

HRESULT CSkill_Monster::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CSkill_Monster::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CSkill_Monster::Priority_Update(_float fTimeDelta)
{
}

void CSkill_Monster::Update(_float fTimeDelta)
{
}

void CSkill_Monster::Late_Update(_float fTimeDelta)
{
}

HRESULT CSkill_Monster::Render()
{
	return S_OK;
}


_float CSkill_Monster::Get_RandomNormalize()
{
	return (_float)rand() / RAND_MAX;
}

_float CSkill_Monster::Get_Random(_float fMin, _float fMax)
{
	return fMin + (fMax - fMin) * Get_RandomNormalize();
}

void CSkill_Monster::Free()
{
	__super::Free();
}
