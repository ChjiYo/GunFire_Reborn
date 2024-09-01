#include "stdafx.h"

#include "Skill_Player.h"
#include "Monster.h"

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

	SKILL_DESC* Desc = static_cast<SKILL_DESC*>(pArg);
	m_pOriginTransform = Desc->pOriginTransform;
	m_vDestoryPos = Desc->vDestoryPos;

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
	list<class CGameObject*> MonStageList1 = m_pGameInstance->Get_Layer(LEVEL_GAMEPLAY, TEXT("Layer_Monster_Stage1"));
	list<class CGameObject*> MonStageList2 = m_pGameInstance->Get_Layer(LEVEL_GAMEPLAY, TEXT("Layer_Monster_Stage2"));
	list<class CGameObject*> MonStageList3 = m_pGameInstance->Get_Layer(LEVEL_BOSS, TEXT("Layer_Monster"));

	for (auto& ColliderIter : MonStageList1)
	{
		CMonster* pMonster = dynamic_cast<CMonster*>(ColliderIter);
	
		if (m_pColliderCom->isCollision(pMonster->Get_Collider(CMonster::COLL_AABB)))
		{
			pMonster->Set_Damage(-5.f);
			m_bDead = true;
		}
	}

	for (auto& ColliderIter : MonStageList2)
	{
		CMonster* pMonster = dynamic_cast<CMonster*>(ColliderIter);

		if (m_pColliderCom->isCollision(pMonster->Get_Collider(CMonster::COLL_AABB)))
		{
			pMonster->Set_Damage(-5.f);
			m_bDead = true;
		}
	}

	for (auto& ColliderIter : MonStageList3)
	{
		CMonster* pMonster = dynamic_cast<CMonster*>(ColliderIter);

		if (m_pColliderCom->isCollision(pMonster->Get_Collider(CMonster::COLL_AABB)))
		{
			pMonster->Set_Damage(-5.f);
			m_bDead = true;
		}
	}

}

HRESULT CSkill_Player::Render()
{
	return S_OK;
}

HRESULT CSkill_Player::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	return S_OK;
}

void CSkill_Player::Set_Position(_float3 vOffSet)
{
	_vector vTargetRight = m_pOriginTransform->Get_State(CTransform::STATE_RIGHT);
	_vector vTargetUp = m_pOriginTransform->Get_State(CTransform::STATE_UP);
	_vector vTargetLook = m_pOriginTransform->Get_State(CTransform::STATE_LOOK);

	m_pTransformCom->Set_State(CTransform::STATE_LOOK, vTargetLook);
	m_pTransformCom->Set_State(CTransform::STATE_UP, vTargetUp);
	m_pTransformCom->Set_State(CTransform::STATE_RIGHT, vTargetRight);

	_vector vSetPos = m_pOriginTransform->Get_State(CTransform::STATE_POSITION) + vTargetRight * vOffSet.x + vTargetUp * vOffSet.y + vTargetLook * vOffSet.z;
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSetW(vSetPos,1.f));

	m_pTransformCom->Radian_Turn(m_pTransformCom->Get_State(CTransform::STATE_UP), XMConvertToRadians(270.f));
}

void CSkill_Player::Free()
{
	__super::Free();
}
