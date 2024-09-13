#include "stdafx.h"
#include "..\Public\Monster.h"

#include "GameInstance.h"

#include "Player.h"
#include "Item.h"
#include "UI_Coin.h"

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
	__super::Priority_Update(fTimeDelta);

	m_fDamageDelay += fTimeDelta;
}

void CMonster::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);

	if (m_pGameInstance->GetLoadingLevelIndex() == LEVEL_GAMEPLAY)
	{
		_float fNNaviHeight = m_pNavigationCom->Navi_Height(m_pTransformCom);
		m_pTransformCom->Gravity(0.1f, fNNaviHeight + 0.25f, fTimeDelta);
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSetY(m_pTransformCom->Get_State(CTransform::STATE_POSITION), fNNaviHeight));
	}
}

void CMonster::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);


	CCollider* pTargetCollider = dynamic_cast<CCollider*>(m_pGameInstance->Get_Component(m_pGameInstance->GetLoadingLevelIndex(), TEXT("Layer_Player"), TEXT("Com_Collider"), 0));
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(m_pGameInstance->Get_Layer(m_pGameInstance->GetLoadingLevelIndex(), TEXT("Layer_Player")).front());

	if (m_pColliderCom[COLL_AABB]->isCollision(pTargetCollider) && m_fDamageDelay > 0.5f)
	{
		pPlayer->Set_Damage(-5);
		m_fDamageDelay = 0.f;
	}
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
	
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_Monster_HpBar"), TEXT("Layer_UI_Monster_CHpBar"),&Desc)))
		return E_FAIL;
	
	return S_OK;
}

HRESULT CMonster::DropItem()
{
	CItem::ITEM_DESC Desc = {};
	Desc.pOriginTransform = m_pTransformCom;
	Desc.fSpeedPerSec = 5.f;
	Desc.fRotationPerSec = XMConvertToRadians(60.f);

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Item_Coin"), TEXT("Layer_Item_Coin"), &Desc)))
		return E_FAIL;

	//dynamic_cast<CUI_Coin*>(m_pGameInstance->Get_Layer(m_pGameInstance->GetCurrentLevelIndex(), TEXT("Layer_UI_Player_UI_Coin")).back())->Set_Coin(rand() % 100 + 10);

	return S_OK;
}

void CMonster::Free()
{
	__super::Free();
}
