#include "Bounding_OBB.h"
#include "DebugDraw.h"

#include "Bounding_AABB.h"
#include "Bounding_Sphere.h"

CBounding_OBB::CBounding_OBB(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CBounding{ pDevice, pContext }
{
}

HRESULT CBounding_OBB::Initialize(CBounding::BOUNDING_DESC* pDesc)
{
	return S_OK;
}

void CBounding_OBB::Update(_fmatrix TransformMatrix)
{
}

HRESULT CBounding_OBB::Render(PrimitiveBatch<VertexPositionColor>* pBatch, _fvector vColor)
{
	return S_OK;
}

_bool CBounding_OBB::isCollision(CCollider::TYPE eTargetColliderType, CBounding* pTargetBounding)
{
	return _bool();
}

_bool CBounding_OBB::Collision_OBB(CBounding_OBB* pTargetDesc)
{
	return _bool();
}

CBounding_OBB::OBB_DESC CBounding_OBB::Compute_OBB()
{
	return OBB_DESC();
}

CBounding_OBB* CBounding_OBB::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CBounding::BOUNDING_DESC* pDesc)
{
	return nullptr;
}

void CBounding_OBB::Free()
{
	__super::Free();
}
