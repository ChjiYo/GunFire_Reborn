#include "stdafx.h"

#include "Skill_Player.h"
#include "Monster.h"
#include <queue>

#include "UI_Number.h"
#include "VFX.h"

#include "Mon_Shield_Knight.h"
#include "Player.h"

#include "Wall_101.h"
#include "Wall_102.h"


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
	list<class CGameObject*> MonStageList4 = m_pGameInstance->Get_Layer(LEVEL_GAMEPLAY, TEXT("Layer_Monster_Shield"));

	list<class CGameObject*> WallList1 = m_pGameInstance->Get_Layer(LEVEL_GAMEPLAY, TEXT("Layer_Wall_101"));
	list<class CGameObject*> WallList2 = m_pGameInstance->Get_Layer(LEVEL_GAMEPLAY, TEXT("Layer_Wall_102"));

	list<class CGameObject*> WallArray[2] = {};

	WallArray[0] = WallList1;
	WallArray[1] = WallList2;

	CPlayer* pPlayer = dynamic_cast<CPlayer*>(m_pGameInstance->Get_Layer(m_pGameInstance->GetLoadingLevelIndex(), TEXT("Layer_Player")).back());

	list<class CGameObject*> MonsterArray[4] = {};
	MonsterArray[0] = MonStageList1;
	MonsterArray[1] = MonStageList2;
	MonsterArray[2] = MonStageList3;
	MonsterArray[3] = MonStageList4;

	m_iDamage = rand() % 10 + m_iSkillDamage;
	CMonster* pMonster = {};

	for (auto& pWall0 : WallArray[0])
	{
		CCollider* pWallColl = dynamic_cast<CWall_101*>(pWall0)->Get_Collider();
	
		if (m_pColliderCom->isCollision(pWallColl))
		{
			CVFX::VFX_DESC Desc = {};
			Desc.vCreatePos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
		
			m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Shield_Hit"), TEXT("Layer_Effect"), &Desc);
		}
	}

	for (auto& pWall1 : WallArray[1])
	{
		CCollider* pWallColl = dynamic_cast<CWall_102*>(pWall1)->Get_Collider();

		if (m_pColliderCom->isCollision(pWallColl))
		{
			CVFX::VFX_DESC Desc = {};
			Desc.vCreatePos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

			m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Shield_Hit"), TEXT("Layer_Effect"), &Desc);
		}
	}
	

	for (auto& Shield : MonStageList4)
	{
		_uint i = dynamic_cast<CMon_Shield_Knight*>(Shield)->Get_MonDesc().iHp;
		
		CCollider* pSheildColl =  dynamic_cast<CCollider*>(dynamic_cast<CMon_Shield_Knight*>(Shield)->Get_Component(CMon_Shield_Knight::PART_WEAPON, TEXT("Com_ColliderAABB")));
	
		if (m_pColliderCom->isCollision(pSheildColl))
		{
			m_pGameInstance->Play_Sound(L"SFX_Sheild", LEVEL_STATIC, false);

			CVFX::VFX_DESC Desc = {};
			Desc.vCreatePos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

			if(pPlayer->Get_WeaponID() != CPlayer::SNIPER)
				m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Shield_Hit"), TEXT("Layer_Effect"),&Desc);

			m_bDead = true;
		}
	}
	

	for (int i = 0; i < 4; ++i)
	{

		for (auto& ColliderIter : MonsterArray[i])
		{
			pMonster = dynamic_cast<CMonster*>(ColliderIter);

			if (!pMonster->Get_Noise())
			{

				if ((pMonster->Get_Collider(CMonster::COLL_AABB) != nullptr) && m_pColliderCom->isCollision(pMonster->Get_Collider(CMonster::COLL_AABB)))
				{
					pMonster->Set_Damage(-m_iDamage);

					CVFX::VFX_DESC VFX_Desc = {};
					VFX_Desc.vCreatePos = pMonster->Get_Transform()->Get_State(CTransform::STATE_POSITION);
					m_pGameInstance->Add_GameObject_ToLayer(m_pGameInstance->GetCurrentLevelIndex(), TEXT("Prototype_GameObject_VFX_Hit"), TEXT("Layer_Effect"), &VFX_Desc);
					m_pMonTransform = pMonster->Get_Transform();

					m_bDead = true;
					m_bFont = true;
					m_bHead = false;

					m_pGameInstance->Play_Sound(L"SFX_Mon_Hit", LEVEL_STATIC, false);
				}
				else if ((pMonster->Get_Collider(CMonster::COLL_SPHERE) != nullptr) && (m_pColliderCom->isCollision(pMonster->Get_Collider(CMonster::COLL_SPHERE))))
				{
					m_iDamage = m_iDamage * 1.5;

					pMonster->Set_Damage(-m_iDamage);

					CVFX::VFX_DESC VFX_Desc = {};
					VFX_Desc.vCreatePos = pMonster->Get_Transform()->Get_State(CTransform::STATE_POSITION);
					m_pGameInstance->Add_GameObject_ToLayer(m_pGameInstance->GetCurrentLevelIndex(), TEXT("Prototype_GameObject_VFX_Hit"), TEXT("Layer_Effect"), &VFX_Desc);
					m_pMonTransform = pMonster->Get_Transform();

					m_bDead = true;
					m_bFont = true;
					m_bHead = true;

					m_pGameInstance->Play_Sound(L"SFX_Mon_Hit", LEVEL_STATIC, false);
				}
			}
		}
	}

	stack<_uint> DamageStack = {};
	_uint iCopyDamage = m_iDamage;
	for (int i = 0; iCopyDamage != 0; ++i)
	{
		DamageStack.push(iCopyDamage % 10);
		iCopyDamage /= 10;
	}

	if (m_bFont)
		FontCreate(pMonster, DamageStack);

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

void CSkill_Player::FontCreate(CMonster* pMonster,stack<_uint> stack)
{
	_uint iNumCnt = { 0 };

	CUI_Number::UI_NUM_DESC Desc = {};
	Desc.iNumUI = log10(m_iDamage) + 1;
	Desc.pTransform = m_pMonTransform;
	Desc.iAngle = rand() % 360;
	Desc.fSpeedPerSec = 5.f;
	Desc.iStatic = iNumber;
	Desc.bHead = m_bHead;

	_tchar AfterPath[MAX_PATH] = {};
	const _tchar* LayerName = TEXT("Layer_UI_Number%d");
	wsprintf(AfterPath, LayerName, iNumber);

	for (int i = 0; i < stack.size(); )
	{
		Desc.iTextureIndex = stack.top();
		Desc.iNumCnt = iNumCnt;
		m_pGameInstance->Add_GameObject_ToLayer(m_pGameInstance->GetLoadingLevelIndex(), TEXT("Prototype_GameObject_UI_Number"), AfterPath, &Desc);
		iNumCnt++;
		stack.pop();
	}

	iNumber++;
	m_bFont = false;
}

void CSkill_Player::Free()
{
	__super::Free();
}
