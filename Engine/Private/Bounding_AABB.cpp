#include "Bounding_AABB.h"
#include "DebugDraw.h"

#include "Bounding_OBB.h"
#include "Bounding_Sphere.h"

CBounding_AABB::CBounding_AABB(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CBounding{ pDevice ,pContext }
{
}

HRESULT CBounding_AABB::Initialize(CBounding::BOUNDING_DESC* pDesc)
{
	BOUNDING_AABB_DESC* pBoundingDesc = static_cast<BOUNDING_AABB_DESC*>(pDesc);

	m_pOriginalDesc = new BoundingBox(pBoundingDesc->vCenter, pBoundingDesc->vExtents);
	m_pDesc = new BoundingBox(*m_pOriginalDesc);

	return S_OK;
}

void CBounding_AABB::Update(_fmatrix TransformMatrix)
{
	_vector		vScale, vRotation, vTranslation;

	XMMatrixDecompose(&vScale, &vRotation, &vTranslation, TransformMatrix);

	_matrix		Transform = /*TransformMatrix*/XMMatrixAffineTransformation(vScale, XMVectorSet(0.f, 0.f, 0.f, 1.f), XMVectorSet(0.f, 0.f, 0.f, 0.f), vTranslation);

	//Transform.r[0] = XMVectorSet(1.f, 0.f, 0.f, 0.f) * XMVector3Length(Transform.r[0]);
	//Transform.r[1] = XMVectorSet(0.f, 1.f, 0.f, 0.f) * XMVector3Length(Transform.r[1]);
	//Transform.r[2] = XMVectorSet(0.f, 0.f, 1.f, 0.f) * XMVector3Length(Transform.r[2]);

	m_pOriginalDesc->Transform(*m_pDesc, Transform);
}

HRESULT CBounding_AABB::Render(PrimitiveBatch<VertexPositionColor>* pBatch, _fvector vColor)
{
	//DX::Draw(pBatch, *m_pDesc, vColor);

	return S_OK;
}

_bool CBounding_AABB::isCollision(CCollider::TYPE eTargetColliderType, CBounding* pTargetBounding)
{
	return _bool();
}

_bool CBounding_AABB::Collision_AABB(CBounding_AABB* pTargetDesc)
{
	return _bool();
}

_float3 CBounding_AABB::Compute_Min()
{
	return _float3();
}

_float3 CBounding_AABB::Compute_Max()
{
	return _float3();
}

CBounding_AABB* CBounding_AABB::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CBounding::BOUNDING_DESC* pDesc)
{
	CBounding_AABB* pInstance = new CBounding_AABB(pDevice, pContext);

	if (FAILED(pInstance->Initialize(pDesc)))
	{
		MSG_BOX(TEXT("Failed to Created : CBounding_AABB"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBounding_AABB::Free()
{
	__super::Free();
}
