#include "Bounding_Ray.h"
#include "DebugDraw.h"

#include "Bounding_AABB.h"
#include "Bounding_OBB.h"
#include "Bounding_Sphere.h"

CBounding_Ray::CBounding_Ray(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CBounding{ pDevice, pContext }
{
}

HRESULT CBounding_Ray::Initialize(CBounding::BOUNDING_DESC* pDesc)
{
	BOUNDING_OBB_DESC* pBoundingDesc = static_cast<BOUNDING_OBB_DESC*>(pDesc);

	_float4			vRotation;
	XMStoreFloat4(&vRotation, XMQuaternionRotationRollPitchYaw(pBoundingDesc->vRotation.x, pBoundingDesc->vRotation.y, pBoundingDesc->vRotation.z));

	m_pOriginalDesc = new BoundingOrientedBox(pBoundingDesc->vCenter, pBoundingDesc->vExtents, vRotation);
	m_pDesc = new BoundingOrientedBox(*m_pOriginalDesc);

	RAY_DESC* pRay = static_cast<RAY_DESC*>(pDesc);
	m_tRayDesc.vOrigin = pRay->vOrigin;
	m_tRayDesc.vDir = pRay->vDir;
	m_tRayDesc.fDist = pRay->fDist;

	return S_OK;
}

void CBounding_Ray::Update(_fmatrix TransformMatrix)
{
	m_pOriginalDesc->Transform(*m_pDesc, TransformMatrix);
}

HRESULT CBounding_Ray::Render(PrimitiveBatch<VertexPositionColor>* pBatch, _fvector vColor)
{
	DX::Draw(pBatch, *m_pDesc, vColor);

	return S_OK;
}

_bool CBounding_Ray::isCollision(CCollider::TYPE eTargetColliderType, CBounding* pTargetBounding)
{
	_bool		isColl = { false };

	switch (eTargetColliderType)
	{
	case CCollider::TYPE_AABB:
		isColl = static_cast<CBounding_AABB*>(pTargetBounding)->Get_Desc().Intersects(m_tRayDesc.vOrigin, XMVector3Normalize(m_tRayDesc.vDir), m_tRayDesc.fDist);
		break;
	case CCollider::TYPE_OBB:
		isColl = static_cast<CBounding_OBB*>(pTargetBounding)->Get_Desc().Intersects(m_tRayDesc.vOrigin, XMVector3Normalize(m_tRayDesc.vDir), m_tRayDesc.fDist);
		break;
	case CCollider::TYPE_SPHERE:
		isColl = static_cast<CBounding_Sphere*>(pTargetBounding)->Get_Desc().Intersects(m_tRayDesc.vOrigin, XMVector3Normalize(m_tRayDesc.vDir), m_tRayDesc.fDist);
		break;
	case CCollider::TYPE_RAY:
		isColl = static_cast<CBounding_Ray*>(pTargetBounding)->Get_Desc().Intersects(m_tRayDesc.vOrigin, XMVector3Normalize(m_tRayDesc.vDir), m_tRayDesc.fDist);
		break;
	}

	return isColl;
}

_bool CBounding_Ray::Collision_OBB(CBounding_Ray* pTargetDesc)
{
	OBB_DESC	OBBDesc[2];

	OBBDesc[0] = Compute_OBB();
	OBBDesc[1] = pTargetDesc->Compute_OBB();

	_float		fDistance[3] = {};

	for (size_t i = 0; i < 2; i++)
	{
		for (size_t j = 0; j < 3; j++)
		{
			fDistance[0] = fabs(XMVectorGetX(XMVector3Dot(XMLoadFloat3(&OBBDesc[1].vCenter) - XMLoadFloat3(&OBBDesc[0].vCenter),
				XMLoadFloat3(&OBBDesc[i].vAlignDir[j]))));

			fDistance[1] = fabs(XMVectorGetX(XMVector3Dot(XMLoadFloat3(&OBBDesc[0].vCenterDir[0]), XMLoadFloat3(&OBBDesc[i].vAlignDir[j])))) +
				fabs(XMVectorGetX(XMVector3Dot(XMLoadFloat3(&OBBDesc[0].vCenterDir[1]), XMLoadFloat3(&OBBDesc[i].vAlignDir[j])))) +
				fabs(XMVectorGetX(XMVector3Dot(XMLoadFloat3(&OBBDesc[0].vCenterDir[2]), XMLoadFloat3(&OBBDesc[i].vAlignDir[j]))));

			fDistance[2] = fabs(XMVectorGetX(XMVector3Dot(XMLoadFloat3(&OBBDesc[1].vCenterDir[0]), XMLoadFloat3(&OBBDesc[i].vAlignDir[j])))) +
				fabs(XMVectorGetX(XMVector3Dot(XMLoadFloat3(&OBBDesc[1].vCenterDir[1]), XMLoadFloat3(&OBBDesc[i].vAlignDir[j])))) +
				fabs(XMVectorGetX(XMVector3Dot(XMLoadFloat3(&OBBDesc[1].vCenterDir[2]), XMLoadFloat3(&OBBDesc[i].vAlignDir[j]))));

			if (fDistance[0] > fDistance[1] + fDistance[2])
				return false;
		}
	}

	return true;
}

CBounding_Ray::OBB_DESC CBounding_Ray::Compute_OBB()
{
	OBB_DESC			OBBDesc{};

	_float3				vPoints[8];

	m_pDesc->GetCorners(vPoints);

	OBBDesc.vCenter = m_pDesc->Center;

	XMStoreFloat3(&OBBDesc.vCenterDir[0], (XMLoadFloat3(&vPoints[5]) - XMLoadFloat3(&vPoints[4])) * 0.5f);
	XMStoreFloat3(&OBBDesc.vCenterDir[1], (XMLoadFloat3(&vPoints[7]) - XMLoadFloat3(&vPoints[4])) * 0.5f);
	XMStoreFloat3(&OBBDesc.vCenterDir[2], (XMLoadFloat3(&vPoints[0]) - XMLoadFloat3(&vPoints[4])) * 0.5f);

	for (size_t i = 0; i < 3; i++)
		XMStoreFloat3(&OBBDesc.vAlignDir[i], XMVector3Normalize(XMLoadFloat3(&OBBDesc.vCenterDir[i])));

	return OBBDesc;
}

CBounding_Ray* CBounding_Ray::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CBounding::BOUNDING_DESC* pDesc)
{
	CBounding_Ray* pInstance = new CBounding_Ray(pDevice, pContext);

	if (FAILED(pInstance->Initialize(pDesc)))
	{
		MSG_BOX(TEXT("Failed to Created : CBounding_Ray"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBounding_Ray::Free()
{
	Safe_Delete(m_pOriginalDesc);
	Safe_Delete(m_pDesc);

	__super::Free();
}
