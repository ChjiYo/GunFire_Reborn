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

	SKILL_DESC* Desc = static_cast<SKILL_DESC*>(pArg);
	m_pTargetTransform = Desc->pTargetTransform;

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_pTargetTransform->Get_State(CTransform::STATE_POSITION));

	_vector vTargetLook = m_pTargetTransform->Get_State(CTransform::STATE_LOOK);
	_vector vTargetUp = m_pTargetTransform->Get_State(CTransform::STATE_UP);
	_vector vTargetRight = m_pTargetTransform->Get_State(CTransform::STATE_RIGHT);

	m_pTransformCom->Set_State(CTransform::STATE_LOOK, vTargetLook);
	m_pTransformCom->Set_State(CTransform::STATE_UP, vTargetUp);
	m_pTransformCom->Set_State(CTransform::STATE_RIGHT, vTargetRight);

	m_pTransformCom->Radian_Turn(m_pTransformCom->Get_State(CTransform::STATE_UP), XMConvertToRadians(270.f));

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
