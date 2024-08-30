#include "stdafx.h"
#include "..\Public\Monster.h"

#include "GameInstance.h"

#include "Player.h"

CMonster::CMonster(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CContainerObject{ pDevice, pContext }
{

}

CMonster::CMonster(const CMonster & Prototype)
	: CContainerObject{ Prototype }
{

}

HRESULT CMonster::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CMonster::Initialize(void * pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	m_pTransformCom->Set_Scaled(0.75f, 0.75f, 0.75f);

	return S_OK;
}

void CMonster::Priority_Update(_float fTimeDelta)
{
	m_fDamageDelay += fTimeDelta;
}

void CMonster::Update(_float fTimeDelta)
{

}

void CMonster::Late_Update(_float fTimeDelta)
{
	CCollider* pTargetCollider = dynamic_cast<CCollider*>(m_pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_Player"), TEXT("Com_Collider"), 0));
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(m_pGameInstance->Get_Layer(LEVEL_GAMEPLAY, TEXT("Layer_Player")).front());

	if (m_pColliderCom[COLL_AABB]->isCollision(pTargetCollider) && m_fDamageDelay > 0.5f)
	{
		pPlayer->Set_Damage(-5.f);
		m_fDamageDelay = 0.f;
	}

	m_pTransformCom->Gravity(0.2f, 0.25f, fTimeDelta);
}

HRESULT CMonster::Render()
{
	return S_OK;
}

HRESULT CMonster::Ready_Components()
{
	CNavigation::NAVIGATION_DESC		NavigationDesc{};
	NavigationDesc.iCurrentCellIndex = 0;

	/* Com_Navigation */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Navigation"),
		TEXT("Com_Navigation"), reinterpret_cast<CComponent**>(&m_pNavigationCom), &NavigationDesc)))
		return E_FAIL;

	
	return S_OK;
}

HRESULT CMonster::Ready_UI()
{
	CUIObject::UI_DESC Desc = {};
	Desc.pTransform = m_pTransformCom;
	Desc.pGameObject = this;

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_Monster_BarPanel"), TEXT("Layer_UI_Monster_ABarPanel"), &Desc)))
		return E_FAIL;
	
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_Monster_HpBar"), TEXT("Layer_UI_Monster_CHpBar"),&Desc)))
		return E_FAIL;
	
	return S_OK;
}

void CMonster::Free()
{
	__super::Free();
}
