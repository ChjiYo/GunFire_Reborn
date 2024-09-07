#include "..\Public\Transform.h"
#include "Shader.h"
#include "Navigation.h"

CTransform::CTransform(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CComponent { pDevice, pContext }
{
}

HRESULT CTransform::Initialize()
{
	
	XMStoreFloat4x4(&m_WorldMatrix, XMMatrixIdentity());

	return S_OK;
}

void CTransform::SetUp_TransformDesc(const TRANSFORM_DESC* pTransformDesc)
{
	m_fSpeedPerSec = pTransformDesc->fSpeedPerSec;
	m_fRotationPerSec = pTransformDesc->fRotationPerSec;
}

HRESULT CTransform::Bind_ShaderResource(CShader * pShader, const _char * pConstantName)
{
	return pShader->Bind_Matrix(pConstantName, &m_WorldMatrix);	
}

void CTransform::Go_Straight(_float fTimeDelta, CNavigation* pNavigation, _bool bMove)
{
	_vector		vPosition = Get_State(STATE_POSITION);
	_vector		vLook = Get_State(STATE_LOOK);	

	vPosition += XMVector3Normalize(vLook) * m_fSpeedPerSec * fTimeDelta;

	if (((nullptr == pNavigation || true == pNavigation->is_Move(vPosition) || bMove)))
	{
		Set_State(STATE_POSITION, vPosition);
		return;
	}
	else if (false == pNavigation->is_Move(vPosition))
	{
		int a = 10;
	}

}

void CTransform::Go_Backward(_float fTimeDelta, CNavigation* pNavigation, _bool bMove)
{
	_vector		vPosition = Get_State(STATE_POSITION);
	_vector		vLook = Get_State(STATE_LOOK);

	vPosition -= XMVector3Normalize(vLook) * m_fSpeedPerSec * fTimeDelta;

	if ((nullptr == pNavigation || true == pNavigation->is_Move(vPosition)|| bMove))
	{
		Set_State(STATE_POSITION, vPosition);
		return;
	}
}

void CTransform::Go_Left(_float fTimeDelta, CNavigation* pNavigation, _bool bMove)
{
	_vector		vPosition = Get_State(STATE_POSITION);
	_vector		vRight = Get_State(STATE_RIGHT);

	
	vPosition -= XMVector3Normalize(vRight) * m_fSpeedPerSec * fTimeDelta;

	if ((nullptr == pNavigation || true == pNavigation->is_Move(vPosition) || bMove))
	{
		Set_State(STATE_POSITION, vPosition);
		return;
	}
}

void CTransform::Go_Right(_float fTimeDelta, CNavigation* pNavigation, _bool bMove)
{
	_vector		vPosition = Get_State(STATE_POSITION);
	_vector		vRight = Get_State(STATE_RIGHT);

	vPosition += XMVector3Normalize(vRight) * m_fSpeedPerSec * fTimeDelta;

	if ((nullptr == pNavigation || true == pNavigation->is_Move(vPosition) || bMove))
	{
		Set_State(STATE_POSITION, vPosition);
		return;
	}
}

void CTransform::Go_StraightY(_float fTimeDelta, CNavigation* pNavigation, _bool bMove)
{
	_vector	vPosition = Get_State(STATE_POSITION);
	_vector	vLook = Get_State(STATE_LOOK);
	_float3 vLookDir = {};
	XMStoreFloat3(&vLookDir, vLook);
	vLookDir.y = 0.f;

	vPosition += XMVector3Normalize(XMLoadFloat3(&vLookDir)) * m_fSpeedPerSec * fTimeDelta;

	if ((nullptr == pNavigation || true == pNavigation->is_Move(vPosition) || bMove) )
	{
		Set_State(STATE_POSITION, vPosition);
		return;
	}
	else
	{
	//vPosition += pNavigation->SliderDIr(vPosition, vLook) * fTimeDelta;
	//Set_State(STATE_POSITION, vPosition);
	//pNavigation->Init_Index(this);
		return;
	}

}

void CTransform::Go_BackwardY(_float fTimeDelta, CNavigation* pNavigation, _bool bMove)
{
	_vector	vPosition = Get_State(STATE_POSITION);
	_vector	vLook = Get_State(STATE_LOOK);
	_float3 vLookDir = {};
	XMStoreFloat3(&vLookDir, vLook);
	vLookDir.y = 0.f;

	vPosition -= XMVector3Normalize(XMLoadFloat3(&vLookDir)) * m_fSpeedPerSec * fTimeDelta;

	if ((nullptr == pNavigation || true == pNavigation->is_Move(vPosition) || bMove))
	{
		Set_State(STATE_POSITION, vPosition);
		return;
	}
}

void CTransform::Go_Up(_float fTimeDelta)
{
	_vector		vPosition = Get_State(STATE_POSITION);
	_vector		vUp = {0.0f , 1.f , 0.f };

	vPosition += XMVector3Normalize(vUp) * m_fSpeedPerSec * fTimeDelta;

	Set_State(STATE_POSITION, vPosition);
}

_bool CTransform::Gravity(_float fWeight, _float fLandPosY, _float fTimeDelta)
{
	_float4 vPosition = {};
	XMStoreFloat4(&vPosition ,Get_State(STATE_POSITION));

	_float4 vDirection = { 0.f, -1.f, 0.f ,1.f};

	m_fGravity_Weight += fWeight;

	vPosition.x += vDirection.x * m_fGravity_Weight * fTimeDelta;
	vPosition.y += vDirection.y * m_fGravity_Weight * fTimeDelta;
	vPosition.z += vDirection.z * m_fGravity_Weight * fTimeDelta;

	if (vPosition.y <= fLandPosY)
	{
		vPosition.y = fLandPosY;
		m_fGravity_Weight = { 0.f };

		return true;
	}
	else 
		Set_State(STATE_POSITION, XMLoadFloat4(&vPosition));

	return false;
}


void CTransform::Turn(_fvector vAxis, _float fTimeDelta)
{
	_vector		vRight = Get_State(STATE_RIGHT);
	_vector		vUp = Get_State(STATE_UP);
	_vector		vLook = Get_State(STATE_LOOK);

	_matrix		RotationMatrix = XMMatrixRotationAxis(vAxis, m_fRotationPerSec * fTimeDelta);

	vRight = XMVector3TransformNormal(vRight, RotationMatrix);
	vUp = XMVector3TransformNormal(vUp, RotationMatrix);
	vLook = XMVector3TransformNormal(vLook, RotationMatrix);

	Set_State(STATE_RIGHT, vRight);
	Set_State(STATE_UP, vUp);
	Set_State(STATE_LOOK, vLook);
}

void CTransform::Rotation(_fvector vAxis, _float fRadian)
{
	m_fRotationY = fRadian;

	_float3		vScale = Get_Scaled();

	_vector		vRight = XMVectorSet(1.f, 0.f, 0.f, 0.f) * vScale.x;
	_vector		vUp = XMVectorSet(0.f, 1.f, 0.f, 0.f) * vScale.y;
	_vector		vLook = XMVectorSet(0.f, 0.f, 1.f, 0.f) * vScale.z;

	_matrix		RotationMatrix = XMMatrixRotationAxis(vAxis, fRadian);

	vRight = XMVector3TransformNormal(vRight, RotationMatrix);
	vUp = XMVector3TransformNormal(vUp, RotationMatrix);
	vLook = XMVector3TransformNormal(vLook, RotationMatrix);

	Set_State(STATE_RIGHT, vRight);
	Set_State(STATE_UP, vUp);
	Set_State(STATE_LOOK, vLook);
}

void CTransform::Rotation(_float3 vRotation)
{
	_vector		Quaternion = XMQuaternionRotationRollPitchYaw(vRotation.x, vRotation.y, vRotation.z);

	_matrix		RotationMatrix = XMMatrixRotationQuaternion(Quaternion);
}

void CTransform::Radian_Turn(_fvector vAxis, _float fRadian)
{
	/* 현재 회전 상태를 얻어온다. */
	_vector		vRight = Get_State(STATE_RIGHT);
	_vector		vUp = Get_State(STATE_UP);
	_vector		vLook = Get_State(STATE_LOOK);

	_matrix		RotationMatrix = XMMatrixRotationAxis(vAxis, fRadian);

	// XMVector3TransformNormal
	Set_State(STATE_RIGHT, XMVector4Transform(vRight, RotationMatrix));
	Set_State(STATE_UP, XMVector4Transform(vUp, RotationMatrix));
	Set_State(STATE_LOOK, XMVector4Transform(vLook, RotationMatrix));
}


void CTransform::LookAt(_fvector vAt)
{
	_vector		vPosition = Get_State(STATE_POSITION);

	_vector		vLook = vAt - vPosition;
	_vector		vRight = XMVector3Cross(XMVectorSet(0.f, 1.f, 0.f, 0.f), vLook);
	_vector		vUp = XMVector3Cross(vLook, vRight);

	_float3		vScaled = Get_Scaled();

	Set_State(STATE_RIGHT, XMVector3Normalize(vRight) * vScaled.x);
	Set_State(STATE_UP, XMVector3Normalize(vUp) * vScaled.y);
	Set_State(STATE_LOOK, XMVector3Normalize(vLook) * vScaled.z);

}

_float CTransform::Look_Degree()
{
	_vector		vLook = Get_State(STATE_LOOK);
	vLook = XMVector3Normalize(vLook);

	_vector		vAxis{ 0.f, 0.f, 1.f,1.0f };

	_vector vAngle = XMVector3Dot(vLook, vAxis);
	_float fAngle = {};
	XMStoreFloat(&fAngle, vAngle);
	fAngle = acos(fAngle);

	_float3 vStoreLook = {};
	XMStoreFloat3(&vStoreLook, vLook);
	if (vStoreLook.x < 0.f)
		fAngle = 3.14f * 2 - fAngle;

	fAngle = XMConvertToDegrees(fAngle);

	return fAngle;
}

_float CTransform::Target_Look_Degree(const _vector& vTargetPos)
{
	_vector vPosition = Get_State(STATE_POSITION);
	_vector vLook = vTargetPos - vPosition;

	vLook = XMVector3Normalize(vLook);

	_vector	vAxis{ 0.f, 0.f, 1.f };

	_vector vAngle =  XMVector3Dot(vLook, vAxis);
	_float fAngle = {};
	XMStoreFloat(&fAngle, vAngle);
	fAngle = acos(fAngle);

	if (XMVectorGetX(vLook) < 0.f)
		fAngle = 3.14f * 2 - fAngle;

	fAngle = XMConvertToDegrees(fAngle);

	return fAngle;
}

CTransform * CTransform::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CTransform*		pInstance = new CTransform(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX(TEXT("Failed to Created : CTransform"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CTransform::Clone(void * pArg)
{
	return nullptr;
}


void CTransform::Free()
{
	__super::Free();

}
