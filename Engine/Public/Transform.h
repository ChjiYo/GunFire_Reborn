#pragma once

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CTransform final : public CComponent
{
public:
	typedef struct
	{
		_float			fSpeedPerSec;
		_float			fRotationPerSec;
	} TRANSFORM_DESC;

public:
	enum STATE { STATE_RIGHT, STATE_UP, STATE_LOOK, STATE_POSITION, STATE_END };

private:
	CTransform(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CTransform() = default;

public:
	/* 저장용 데이터 -> 연산용 데이터 XMLoad */
	/* 연산용 데이터 -> 저장용 데이터 XMStore */

	_vector Get_State(STATE eState) {
		return XMLoadFloat4x4(&m_WorldMatrix).r[eState];
	}

	_float3 Get_Scaled() const {
		_matrix		WorldMatrix = XMLoadFloat4x4(&m_WorldMatrix);
		return _float3(XMVectorGetX(XMVector3Length(WorldMatrix.r[STATE_RIGHT])),
			XMVectorGetX(XMVector3Length(WorldMatrix.r[STATE_UP])),
			XMVectorGetX(XMVector3Length(WorldMatrix.r[STATE_LOOK])));
	}

	_matrix Get_WorldMatrix() const {
		return XMLoadFloat4x4(&m_WorldMatrix);
	}

	const _float4x4* Get_WorldMatrixPtr() const {
		return &m_WorldMatrix;
	}

	_matrix Get_WorldMatrix_Inverse() const {
		return XMMatrixInverse(nullptr, XMLoadFloat4x4(&m_WorldMatrix));
	}

	_float Get_RotationYValue() { return m_fRotationY; }

public:

	void Set_Scaled(_float fScaleX, _float fScaleY, _float fScaleZ) {
		XMStoreFloat4((_float4*)&m_WorldMatrix.m[STATE_RIGHT][0], XMVector3Normalize(Get_State(STATE_RIGHT)) * fScaleX);
		XMStoreFloat4((_float4*)&m_WorldMatrix.m[STATE_UP][0], XMVector3Normalize(Get_State(STATE_UP)) * fScaleY);
		XMStoreFloat4((_float4*)&m_WorldMatrix.m[STATE_LOOK][0], XMVector3Normalize(Get_State(STATE_LOOK)) * fScaleZ);
	}
	

	void Set_State(STATE eState, _fvector vState) {
		XMStoreFloat4((_float4*)&m_WorldMatrix.m[eState][0], vState);
	}

public:
	HRESULT Initialize();
	void SetUp_TransformDesc(const TRANSFORM_DESC* pTransformDesc);
	HRESULT Bind_ShaderResource(class CShader* pShader, const _char* pConstantName);

public:
	/* Transform_Position */
	void Go_Straight(_float fTimeDelta, class CNavigation* pNavigation = nullptr);
	void Go_Backward(_float fTimeDelta, class CNavigation* pNavigation = nullptr);
	void Go_Left(_float fTimeDelta, class CNavigation* pNavigation = nullptr);
	void Go_Right(_float fTimeDelta, class CNavigation* pNavigation = nullptr);

	void Go_StraightY(_float fTimeDelta, class  CNavigation* pNavigation = nullptr);
	void Go_BackwardY(_float fTimeDelta, class  CNavigation* pNavigation = nullptr);

	void Go_Up(_float fTimeDelta);

	void Set_Speed(_float fSpeed) { m_fSpeedPerSec = fSpeed; }
	_float Get_Speed() { return m_fSpeedPerSec; }

	_bool CTransform::Gravity(_float fWeight, _float fLandPosY, _float fTimeDelta);
public:
	/* Transform_Rotation */
	void Turn(_fvector vAxis, _float fTimeDelta);
	void Rotation(_fvector vAxis, _float fRadian);
	void Rotation(_float3 vRotation);
	void Radian_Turn(_fvector vAxis, _float fRadian);
	void LookAt(_fvector vAt);

	_float Look_Degree();
	_float Target_Look_Degree(const _vector& vTargetPos);

private:
	_float4x4				m_WorldMatrix = {};
	_float					m_fSpeedPerSec = {};
	_float					m_fRotationPerSec = {};
	_float					m_fGravity_Weight = {};
	_float					m_fRotationY = {};
   
public:
	static CTransform* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END
