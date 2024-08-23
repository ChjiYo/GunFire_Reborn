#pragma once

#include "Bounding.h"

BEGIN(Engine)

class CBounding_Ray final : public CBounding
{
public:
	typedef struct : public BOUNDING_DESC
	{
		_float3		vExtents;
		_float3		vRotation;
	}BOUNDING_OBB_DESC;

	typedef struct : public BOUNDING_DESC
	{
		_vector vOrigin = {};
		_vector vDir = {};
		_float fDist = {};
		_float3		vExtents;
		_float3		vRotation;
	}BOUNDING_RAY_DESC;

private:
	CBounding_Ray(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CBounding_Ray() = default;

public:
	BoundingOrientedBox Get_Desc() const {
		return *m_pDesc;
	}

	BOUNDING_RAY_DESC Get_Ray() const {
		return m_tRayDesc;
	}

public:
	virtual HRESULT Initialize(CBounding::BOUNDING_DESC* pDesc);
	virtual void Update(_fmatrix TransformMatrix) override;
	virtual HRESULT Render(PrimitiveBatch<VertexPositionColor>* pBatch, _fvector vColor) override;
	virtual _bool isCollision(CCollider::TYPE eTargetColliderType, CBounding* pTargetBounding) override;

private:
	BoundingOrientedBox* m_pOriginalDesc = { nullptr };
	BoundingOrientedBox* m_pDesc = { nullptr };

private:
	BOUNDING_RAY_DESC m_tRayDesc = {};

public:
	static CBounding_Ray* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CBounding::BOUNDING_DESC* pDesc);
	virtual void Free() override;
};

END

