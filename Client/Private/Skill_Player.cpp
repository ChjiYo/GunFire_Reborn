#include "stdafx.h"

#include "Skill_Player.h"

CSkill_Player::CSkill_Player(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CSkill{ pDevice  , pContext }
{
}

CSkill_Player::CSkill_Player(const CSkill& Prototype)
	:CSkill{ Prototype }
{
}

HRESULT CSkill_Player::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CSkill_Player::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CSkill_Player::Priority_Update(_float fTimeDelta)
{
}

void CSkill_Player::Update(_float fTimeDelta)
{
}

void CSkill_Player::Late_Update(_float fTimeDelta)
{
}

HRESULT CSkill_Player::Render()
{
	return S_OK;
}

void CSkill_Player::Free()
{
	__super::Free();
}
