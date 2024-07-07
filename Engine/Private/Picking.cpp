#include "..\Public\Picking.h"
#include "GameInstance.h"

#include "DirectXCollision.h"
#include <DirectXMath.h>

using namespace DirectX;

CPicking::CPicking(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: m_pDevice{ pDevice },
	m_pContext{ pContext },
	m_pGameInstance{ CGameInstance::Get_Instance() }
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
	Safe_AddRef(m_pGameInstance);
}

HRESULT CPicking::Initialize(HWND hWnd)
{
	m_hWnd = hWnd;
	return S_OK;
}

void CPicking::Update()
{
    D3D11_VIEWPORT ViewportDesc{};
	UINT numViewports = 1;
	m_pContext->RSGetViewports(&numViewports, &ViewportDesc);

	if (numViewports > 0) {
		D3D11_VIEWPORT* viewports = new D3D11_VIEWPORT[numViewports];
		m_pContext->RSGetViewports(&numViewports, viewports);

		// 첫 번째 뷰포트를 가져옵니다. (여러 뷰포트가 있을 수 있으므로 필요에 따라 수정해야 합니다)
		D3D11_VIEWPORT viewport = viewports[0];

		// 사용이 끝난 후 메모리 해제
		delete[] viewports;
	}

	POINT ptMouse{};
	GetCursorPos(&ptMouse);
	ScreenToClient(m_hWnd, &ptMouse);

	_float3 vMousePos;
	vMousePos.x = ptMouse.x / (ViewportDesc.Width * 0.5f) - 1.f;
	vMousePos.y = ptMouse.y / -(ViewportDesc.Height * 0.5f) + 1.f;
	vMousePos.z = 0.f;

	_matrix ViewMatrixInv, ProjMatrixInv;

	ProjMatrixInv = m_pGameInstance->Get_Transform_Inverse_Matrix(CPipeLine::D3DTS_PROJ);

	/* 뷰스페이스 상의 RayDir, RayPos를 구하자. */
	XMVECTOR vMousePosVector = XMLoadFloat3(&vMousePos);
	vMousePosVector = XMVector3TransformCoord(vMousePosVector, ProjMatrixInv);

	m_vRayPos = _float3{ 0.f, 0.f, 0.f };
	m_vRayDir.x = XMVectorGetX(vMousePosVector) - m_vRayPos.x;
	m_vRayDir.y = XMVectorGetY(vMousePosVector) - m_vRayPos.y;
	m_vRayDir.z = XMVectorGetZ(vMousePosVector) - m_vRayPos.z;

	ViewMatrixInv = m_pGameInstance->Get_Transform_Inverse_Matrix(CPipeLine::D3DTS_VIEW);

	XMVECTOR vRayDirVector = XMLoadFloat3(&m_vRayDir);
	vRayDirVector = XMVector3TransformNormal(vRayDirVector, ViewMatrixInv);
	vRayDirVector = XMVector3Normalize(vRayDirVector);
	XMStoreFloat3(&m_vRayDir, vRayDirVector);

	XMVECTOR vRayPosVector = XMLoadFloat3(&m_vRayPos);
	vRayPosVector = XMVector3TransformCoord(vRayPosVector, ViewMatrixInv);
	XMStoreFloat3(&m_vRayPos,vRayPosVector);

	_float vbs = 13;
}

void CPicking::Transform_ToLocalSpace(const XMMATRIX* pWorldMatrix)
{
	// pWorldMatrix의 역행렬을 구합니다.
	XMMATRIX WorldMatrixInv = XMMatrixInverse(nullptr, *pWorldMatrix);

	// 광선의 방향 벡터를 로컬 공간으로 변환하고 정규화합니다.
	XMVECTOR vRayDir = XMLoadFloat3(&m_vRayDir);
	XMVECTOR vRayDirLocal = XMVector3TransformNormal(vRayDir, WorldMatrixInv);
	vRayDirLocal = XMVector3Normalize(vRayDirLocal);
	XMStoreFloat3(&m_vRayDir_Local, vRayDirLocal);

	// 광선의 위치 벡터를 로컬 공간으로 변환합니다.
	XMVECTOR vRayPos = XMLoadFloat3(&m_vRayPos);
	XMVECTOR vRayPosLocal = XMVector3TransformCoord(vRayPos, WorldMatrixInv);
	XMStoreFloat3(&m_vRayPos_Local, vRayPosLocal);


	float fslwp = 10.f;
}

_bool CPicking::Picked_InLocalSpace(const _vector* pPointA, const _vector* pPointB, const _vector* pPointC, _vector* pPickPos)
{
	_float fDist = {};
	_bool isPicked = { false };

	_vector vRayPos = {};
	_vector vRayDir = {};

	vRayPos = XMLoadFloat3(&m_vRayPos_Local);
	vRayDir = XMLoadFloat3(&m_vRayDir_Local);

	isPicked = TriangleTests::Intersects(vRayPos, vRayDir, *pPointA, *pPointB, *pPointC, fDist);

	if (isPicked == true)
	{
		*pPickPos = XMLoadFloat3(&m_vRayPos_Local) + XMVector3Normalize(XMLoadFloat3(&m_vRayDir_Local))  * fDist;
	}

	return isPicked;
}

_bool CPicking::Picked_InWorldSpace(_vector pPointA, _vector pPointB, _vector pPointC, _vector* pPickPos)
{
	_float fDist = 0.f;
	_bool isPicked = TriangleTests::Intersects(XMLoadFloat3(&m_vRayPos), XMLoadFloat3(&m_vRayDir), pPointA, pPointB, pPointC, fDist);

	if (isPicked)
	{
		*pPickPos = XMLoadFloat3(&m_vRayPos) + XMVector3Normalize(XMLoadFloat3(&m_vRayDir)) * fDist;
	}

	return  isPicked;
}

CPicking* CPicking::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, HWND hWnd)
{
	CPicking* pInstance = new CPicking(pDevice, pContext);

	if (FAILED(pInstance->Initialize(hWnd)))
	{
		MSG_BOX(TEXT("Failed to Created : CPicking"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPicking::Free()
{
	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
	Safe_Release(m_pGameInstance);

	__super::Free();
}