#include "stdafx.h"

#include "Skill_Monster.h"
#include "GameInstance.h"

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

	/*SKILL_DESC* Desc = static_cast<SKILL_DESC*>(pArg);
	m_pOriginTransform = Desc->pOriginTransform;

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_pOriginTransform->Get_State(CTransform::STATE_POSITION));

	_vector vTargetLook = m_pOriginTransform->Get_State(CTransform::STATE_LOOK);
	_vector vTargetUp = m_pOriginTransform->Get_State(CTransform::STATE_UP);
	_vector vTargetRight = m_pOriginTransform->Get_State(CTransform::STATE_RIGHT);

	m_pTransformCom->Set_State(CTransform::STATE_LOOK, vTargetLook);
	m_pTransformCom->Set_State(CTransform::STATE_UP, vTargetUp);
	m_pTransformCom->Set_State(CTransform::STATE_RIGHT, vTargetRight);

	m_pTransformCom->Radian_Turn(m_pTransformCom->Get_State(CTransform::STATE_UP), XMConvertToRadians(270.f));*/

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
