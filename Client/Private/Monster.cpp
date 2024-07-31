#include "stdafx.h"
#include "..\Public\Monster.h"

#include "GameInstance.h"

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

	return S_OK;
}

void CMonster::Priority_Update(_float fTimeDelta)
{
		
}

void CMonster::Update(_float fTimeDelta)
{

}

void CMonster::Late_Update(_float fTimeDelta)
{
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
	NavigationDesc.vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	/* Com_Navigation */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Navigation"),
		TEXT("Com_Navigation"), reinterpret_cast<CComponent**>(&m_pNavigationCom), &NavigationDesc)))
		return E_FAIL;

	/* For.Com_Collider */
	CBounding_AABB::BOUNDING_AABB_DESC	BoundingDesc{};

	BoundingDesc.vExtents = _float3(0.5f, 0.5f, 0.5f);
	BoundingDesc.vCenter = _float3(0.f, BoundingDesc.vExtents.y, 0.f);
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_AABB"),
		TEXT("Com_ColliderAABB"), reinterpret_cast<CComponent**>(&m_pColliderCom[COLL_AABB]), &BoundingDesc)))
		return E_FAIL;

	CBounding_Sphere::BOUNDING_SPHERE_DESC	BoundingSphereDesc{};

	BoundingSphereDesc.fRadius = 0.5f;
	BoundingSphereDesc.vCenter = _float3(0.f, BoundingSphereDesc.fRadius + 1.f, 0.f);


	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_Sphere"),
		TEXT("Com_Collider_Sphere"), reinterpret_cast<CComponent**>(&m_pColliderCom[COLL_SPHERE]), &BoundingSphereDesc)))
		return E_FAIL;

	CBounding_Sphere::BOUNDING_SPHERE_DESC	RangeDesc{};

	RangeDesc.fRadius = 10.f;
	RangeDesc.vCenter = _float3(0.f, RangeDesc.fRadius / 2.f , 0.f);

	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_Sphere"),
		TEXT("Com_Collider_Range"), reinterpret_cast<CComponent**>(&m_pRangeCom), &RangeDesc)))
		return E_FAIL;
	

	return S_OK;
}

void CMonster::Free()
{
	Safe_Release(m_pRangeCom);

	__super::Free();
}
