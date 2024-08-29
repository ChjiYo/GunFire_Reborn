#pragma once

#include "Base.h"

BEGIN(Engine)

class CFrustum final : public CBase
{
private:
	CFrustum();
	virtual ~CFrustum() = default;

public:
	HRESULT Initialize();
	void Update();

	void Transform_ToLocalSpace(_fmatrix WorldMatrixInverse);
	_bool isIn_InWorldSpace(_fvector vWorldPosition, _float fRadius);
	_bool isIn_InLocalSpace(_fvector vLocalPosition, _float fRadius);

private:
	class CGameInstance* m_pGameInstance = { nullptr };
	_float3					m_vPoints[8];
	_float3					m_vWorldPoints[8];
	_float4					m_vWorldPlanes[6];
	_float4					m_vLocalPlanes[6];



private:
	void Make_Planes(_float4* pPlanes, const _float3* pPoints);

public:
	static CFrustum* Create();
	virtual void Free() override;
};

END