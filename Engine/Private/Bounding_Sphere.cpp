#include "Bounding_Sphere.h"
#include "DebugDraw.h"

#include "Bounding_AABB.h"
#include "Bounding_OBB.h"

CBounding_Sphere::CBounding_Sphere(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CBounding{ pDevice, pContext }
{
}

HRESULT CBounding_Sphere::Initialize(CBounding::BOUNDING_DESC* pDesc)
{
	return S_OK;
}

void CBounding_Sphere::Update(_fmatrix TransformMatrix)
{
}

HRESULT CBounding_Sphere::Render(PrimitiveBatch<VertexPositionColor>* pBatch, _fvector vColor)
{
	return S_OK;
}

_bool CBounding_Sphere::isCollision(CCollider::TYPE eTargetColliderType, CBounding* pTargetBounding)
{
	return _bool();
}

CBounding_Sphere* CBounding_Sphere::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CBounding::BOUNDING_DESC* pDesc)
{
	CBounding_Sphere* pInstance = new CBounding_Sphere(pDevice, pContext);

	if (FAILED(pInstance->Initialize(pDesc)))
	{
		MSG_BOX(TEXT("Failed to Created : CBounding_Sphere"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBounding_Sphere::Free()
{
	__super::Free();
}
