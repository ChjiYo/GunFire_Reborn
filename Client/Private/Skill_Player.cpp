#include "stdafx.h"

#include "Skill_Player.h"
#include "Monster.h"
#include <queue>
#include <stack>

#include "UI_Number.h"
#include "VFX.h"

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
	m_iSkillDamage = Desc->iSkillDamage;

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

	_int iDamage = rand() % 10 + m_iSkillDamage;
	_uint iCopyDamage = iDamage;
	_uint iNumCnt = { 0 };

	stack<_uint> DamageStack = {};
	for (int i = 0; iCopyDamage != 0 ; ++i)
	{
		DamageStack.push(iCopyDamage % 10);
		iCopyDamage /= 10;
	}

	for (auto& ColliderIter : MonStageList1)
	{
		CMonster* pMonster = dynamic_cast<CMonster*>(ColliderIter);
	
		if (m_pColliderCom->isCollision(pMonster->Get_Collider(CMonster::COLL_AABB)))
		{
			pMonster->Set_Damage(-iDamage);

			CUI_Number::UI_NUM_DESC Desc = {};
			Desc.iNumUI = log10(iDamage) + 1;
			Desc.pTransform = pMonster->Get_Transform();
			Desc.iAngle = rand() % 360;
			Desc.fSpeedPerSec = 5.f;

			for (int i = 0; i < DamageStack.size(); )
			{
				Desc.iTextureIndex = DamageStack.top();
				Desc.iNumCnt = iNumCnt;
				m_pGameInstance->Add_GameObject_ToLayer(m_pGameInstance->GetLoadingLevelIndex(), TEXT("Prototype_GameObject_UI_Number"), TEXT("Layer_UI_Number"), &Desc);
				iNumCnt++;
				DamageStack.pop();
			}

			CVFX::VFX_DESC VFX_Desc = {};
			VFX_Desc.vCreatePos = pMonster->Get_Transform()->Get_State(CTransform::STATE_POSITION);
			m_pGameInstance->Add_GameObject_ToLayer(m_pGameInstance->GetCurrentLevelIndex(), TEXT("Prototype_GameObject_VFX_Hit"), TEXT("Layer_Effect"),&VFX_Desc);

			m_bDead = true;
		}
	}

	for (auto& ColliderIter : MonStageList2)
	{
		CMonster* pMonster = dynamic_cast<CMonster*>(ColliderIter);

		if (m_pColliderCom->isCollision(pMonster->Get_Collider(CMonster::COLL_AABB)))
		{
			pMonster->Set_Damage(-iDamage);

			CUI_Number::UI_NUM_DESC Desc = {};
			Desc.iNumUI = log10(iDamage) + 1;
			Desc.pTransform = pMonster->Get_Transform();
			Desc.iAngle = rand() % 360;
			Desc.fSpeedPerSec = 5.f;

			for (int i = 0; i < DamageStack.size(); )
			{
				Desc.iTextureIndex = DamageStack.top();
				Desc.iNumCnt = iNumCnt;
				m_pGameInstance->Add_GameObject_ToLayer(m_pGameInstance->GetLoadingLevelIndex(), TEXT("Prototype_GameObject_UI_Number"), TEXT("Layer_UI_Number"), &Desc);
				iNumCnt++;
				DamageStack.pop();
			}

			CVFX::VFX_DESC VFX_Desc = {};
			VFX_Desc.vCreatePos = XMLoadFloat4(&m_vDestoryPos);
			m_pGameInstance->Add_GameObject_ToLayer(m_pGameInstance->GetCurrentLevelIndex(), TEXT("Prototype_GameObject_VFX_Hit"), TEXT("Layer_Effect"), &VFX_Desc);

			m_bDead = true;
		}
	}

	for (auto& ColliderIter : MonStageList3)
	{
		CMonster* pMonster = dynamic_cast<CMonster*>(ColliderIter);

		if (m_pColliderCom->isCollision(pMonster->Get_Collider(CMonster::COLL_AABB)))
		{
			pMonster->Set_Damage(-iDamage);
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
