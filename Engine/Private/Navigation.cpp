#include "..\Public\Navigation.h"
#include "Cell.h"
#include "Shader.h"
#include "GameInstance.h"

_float4x4	CNavigation::m_WorldMatrix{};

CNavigation::CNavigation(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CComponent { pDevice, pContext }
{
}

CNavigation::CNavigation(const CNavigation & Prototype)
	: CComponent{ Prototype }
	, m_Cells{ Prototype.m_Cells }
	, m_iCurrentCellIndex{ Prototype.m_iCurrentCellIndex }
#ifdef _DEBUG
	, m_pShader { Prototype.m_pShader }
#endif
{
#ifdef _DEBUG
	Safe_AddRef(m_pShader);
#endif

	for (auto& pCell : m_Cells)
		Safe_AddRef(pCell);
}

HRESULT CNavigation::Initialize_Prototype(const _tchar * pNavigationDataFilePath)
{
	_ulong			dwByte = { 0 };

	HANDLE			hFile = CreateFile(pNavigationDataFilePath, GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (0 == hFile)
		return E_FAIL;

	_float3			vPoints[3] = {};
	
	while (true)
	{

 		ReadFile(hFile, vPoints, sizeof(_float3) * 3, &dwByte, nullptr);
		if (0 == dwByte)
			break;

		CCell*			pCell = CCell::Create(m_pDevice, m_pContext, vPoints, m_Cells.size());
		if (nullptr == pCell)
			return E_FAIL;

		m_Cells.emplace_back(pCell);
	}

	CloseHandle(hFile);

	Make_Neighbors();

#ifdef _DEBUG

	/* 경로는? : 클라이언트에서 이 엔진을 사용하고자했다면 엔진에서 제공하는 쉐이더 파일을 클라이언트 프로젝트의 어떤 위치에 배치해놓을 것이다. */
	m_pShader = CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_Cell.hlsl"), VTXPOS::Elements, VTXPOS::iNumElements);
	if (nullptr == m_pShader)
		return E_FAIL;

#endif

	return S_OK;
}

HRESULT CNavigation::Initialize(void * pArg)
{
	if (nullptr != pArg)
	{
		NAVIGATION_DESC*		pDesc = static_cast<NAVIGATION_DESC*>(pArg);
		m_iCurrentCellIndex = pDesc->iCurrentCellIndex;
	}

	return S_OK;
}

void CNavigation::Update(_fmatrix TerrainWorldMatrix)
{
	

	XMStoreFloat4x4(&m_WorldMatrix, TerrainWorldMatrix);
}

HRESULT CNavigation::Render()
{
#ifdef _DEBUG
	if (FAILED(m_pShader->Bind_Matrix("g_WorldMatrix", &m_WorldMatrix)))
		return E_FAIL;
	if (FAILED(m_pShader->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;
	if (FAILED(m_pShader->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;

	if (m_pShader->Bind_RawValue("g_vColor", &(m_iCurrentCellIndex == -1 ? _float4(0.f, 1.f, 0.f, 1.f) : _float4(1.f, 0.f, 0.f, 1.f)), sizeof(_float4)))
		return E_FAIL;

	if (-1 == m_iCurrentCellIndex)
	{
		m_pShader->Begin(0);

		for (auto& pCell : m_Cells)
		{
			if (nullptr != pCell)
				pCell->Render();
		}
	}

	else
	{
		_float4x4		WorldMatrix = m_WorldMatrix;
		WorldMatrix.m[3][1] += 0.5f;
		if (FAILED(m_pShader->Bind_Matrix("g_WorldMatrix", &WorldMatrix)))
			return E_FAIL;

		m_pShader->Begin(0);

		m_Cells[m_iCurrentCellIndex]->Render();
	}

#endif
	return S_OK;
}

_bool CNavigation::is_Move(_fvector vPosition)
{
	if (-1 == m_iCurrentCellIndex)
		return false;

	_vector		vLocalPos = XMVector3TransformCoord(vPosition, XMMatrixInverse(nullptr, XMLoadFloat4x4(&m_WorldMatrix)));

	_int		iNeighborIndex = { -1 };

	if (true == m_Cells[m_iCurrentCellIndex]->isIn(vLocalPos, &iNeighborIndex))
	{
		return true;
	}
	/* 움직이고난 결과위치가 이 쎌 밖으로 나갔다.*/
	else
	{
		/* 나간 위치에 셀이 있다면? */
		if (-1 != iNeighborIndex)
		{
			while (true)
			{
				if (-1 == iNeighborIndex)
					return false;

				if (true == m_Cells[iNeighborIndex]->isIn(vLocalPos, &iNeighborIndex))
					break;
			}

			m_iCurrentCellIndex = iNeighborIndex;
			return true;
		}

		/* 나간 위치에 셀이 없다면? */
		else
		{
			return false;
		}
	}
}

HRESULT CNavigation::Add_Sell(_float3 fPoint[3])
{
	for (auto& iter : m_Cells)
	{
		for (int i = 0; i < 3; ++i)
		{
			for (int k = 0; k < 3; ++k)
			{
				_float fPosX =  fPoint[i].x - iter->Get_Point(k).x;
				_float fPosY = fPoint[i].y - iter->Get_Point(k).y;
				_float fPosZ = fPoint[i].z - iter->Get_Point(k).z;
			
				if((fPosX > -0.5f && fPosX < 0.5f) && (fPosY > -0.5f && fPosY < 0.5f) && (fPosZ > -0.5f && fPosZ < 0.5f))
					fPoint[i] =	iter->Get_Point(k);
			}
		}
	}

	_vector vPointA = { fPoint[0].x , fPoint[0].y ,fPoint[0].z };
	_vector  vPointB = { fPoint[1].x,fPoint[1].y ,fPoint[1].z };
	_vector  vPointC = { fPoint[2].x , fPoint[2].y , fPoint[2].z };

	_vector  vLineAB = XMVectorSubtract(vPointB , vPointA);
	_vector  vLineAC = XMVectorSubtract(vPointC , vPointA);
	XMVECTOR  vNormal = XMVector3Cross(vLineAB, vLineAC);

	_float fNormalY = XMVectorGetY(vNormal);

	if (!(fNormalY > 0))
	{
		swap(fPoint[0], fPoint[1]);
	}

	CCell* pCell = CCell::Create(m_pDevice, m_pContext, fPoint, m_Cells.size());
	if (nullptr == pCell)
		return E_FAIL;

	m_Cells.emplace_back(pCell);

	return S_OK;
}

void CNavigation::Make_Neighbors()
{
	for (auto& pSourCell : m_Cells)
	{
		for (auto& pDestCell : m_Cells)
		{
			if (pSourCell == pDestCell)
				continue;

			if (true == pDestCell->Compare_Point(pSourCell->Get_Point(CCell::POINT_A), pSourCell->Get_Point(CCell::POINT_B)))
				pSourCell->Set_Neighbor(CCell::LINE_AB, pDestCell);

			if (true == pDestCell->Compare_Point(pSourCell->Get_Point(CCell::POINT_B), pSourCell->Get_Point(CCell::POINT_C)))
				pSourCell->Set_Neighbor(CCell::LINE_BC, pDestCell);

			if (true == pDestCell->Compare_Point(pSourCell->Get_Point(CCell::POINT_C), pSourCell->Get_Point(CCell::POINT_A)))
				pSourCell->Set_Neighbor(CCell::LINE_CA, pDestCell);
		}
	}
}

_uint CNavigation::Init_Index(class CTransform* pTransform)
{
	_float3 vPos = {};
	XMStoreFloat3(&vPos, pTransform->Get_State(CTransform::STATE_POSITION));


	for (int i = 0; i < m_Cells.size(); ++i)
	{
		_float2								m_vPosX = {};
		_float2								m_vPosZ = {};

		m_vPosX.y = m_Cells.at(i)->Get_Point(0).x;
		m_vPosZ.y = m_Cells.at(i)->Get_Point(0).z;

		for (int k = 0; k < 3; ++k)
		{
			m_vPosX.x = max(m_vPosX.x, m_Cells.at(i)->Get_Point(k).x);
			m_vPosX.y = min(m_vPosX.y, m_Cells.at(i)->Get_Point(k).x);

			m_vPosZ.x = max(m_vPosZ.x, m_Cells.at(i)->Get_Point(k).z);
			m_vPosZ.y = min(m_vPosZ.y, m_Cells.at(i)->Get_Point(k).z);
		}

			
		if ((m_vPosX.y < vPos.x && vPos.x < m_vPosX.x) && (m_vPosZ.y < vPos.z && vPos.z < m_vPosZ.x))
		{
			m_iCurrentCellIndex = i + 1;
			return m_iCurrentCellIndex;
		}
	}
}

_float CNavigation::Navi_Height(class CTransform* pTransform)
{

	_float fPosX = XMVectorGetX(pTransform->Get_State(CTransform::STATE_POSITION));
	_float fPosZ = XMVectorGetZ(pTransform->Get_State(CTransform::STATE_POSITION));

	_vector vPosA = XMLoadFloat3(&m_Cells.at(m_iCurrentCellIndex)->Get_Point(0));
	_vector vPosB = XMLoadFloat3(&m_Cells.at(m_iCurrentCellIndex)->Get_Point(1));
	_vector vPosC = XMLoadFloat3(&m_Cells.at(m_iCurrentCellIndex)->Get_Point(2));

	_vector v1 = vPosB - vPosA;
	_vector v2 = vPosC - vPosA;
	_vector vNormal = XMVector3Cross(v1, v2);

	// 평면 방정식의 계수 계산
	float fPlaneX = XMVectorGetX(vNormal);
	float fPlaneY = XMVectorGetY(vNormal);
	float fPlaneZ = XMVectorGetZ(vNormal);

	// 절편 d 계산
	float fPlaneW = -(fPlaneX * XMVectorGetX(vPosA) + fPlaneY * XMVectorGetY(vPosA) + fPlaneZ * XMVectorGetZ(vPosA));

	// 주어진 x, z 위치에서 y 계산
	float fHeight = -(fPlaneX * fPosX + fPlaneZ * fPosZ + fPlaneW) / fPlaneY;

	return fHeight;
}

HRESULT CNavigation::Save_Navi()
{
	_ulong			dwByte = { 0 };

	HANDLE			hFile = CreateFile(TEXT("../Bin/DataFiles/Navigation.dat"), GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
	if (0 == hFile)
		return E_FAIL;

	_float3			vPoints[3];
	
	for (auto& iter : m_Cells)
	{
		vPoints[0] = iter->Get_Point(0);
		vPoints[1] = iter->Get_Point(1);
		vPoints[2] = iter->Get_Point(2);

		WriteFile(hFile, vPoints, sizeof(_float3) * 3, &dwByte, nullptr);
	}

	CloseHandle(hFile);

	return S_OK; 
}

HRESULT CNavigation::Delete_Navi()
{
	if (m_Cells.size() == 0)
		return E_FAIL;

	Safe_Release(m_Cells.back());
	m_Cells.pop_back();

	return S_OK;
}

CNavigation * CNavigation::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, const _tchar * pNavigationDataFilePath)
{
	CNavigation*		pInstance = new CNavigation(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(pNavigationDataFilePath)))
	{
		MSG_BOX(TEXT("Failed to Created : CNavigation"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CNavigation::Clone(void * pArg)
{
	CNavigation*		pInstance = new CNavigation(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CNavigation"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CNavigation::Free()
{
#ifdef _DEBUG
	Safe_Release(m_pShader);
#endif

	for (auto& pCell : m_Cells)
		Safe_Release(pCell);

	m_Cells.clear();

	__super::Free();
}
