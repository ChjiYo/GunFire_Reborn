#include "..\Public\VIBuffer_Instancing.h"

CVIBuffer_Instancing::CVIBuffer_Instancing(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CVIBuffer{ pDevice, pContext }
{
}

CVIBuffer_Instancing::CVIBuffer_Instancing(const CVIBuffer_Instancing& Prototype)
	: CVIBuffer{ Prototype }
	, m_pVBInstance{ Prototype.m_pVBInstance }
	, m_iNumInstance{ Prototype.m_iNumInstance }
	, m_iInstanceVertexStride{ Prototype.m_iInstanceVertexStride }
	, m_iNumIndexPerInstance{ Prototype.m_iNumIndexPerInstance }
{
	Safe_AddRef(m_pVBInstance);
}

HRESULT CVIBuffer_Instancing::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CVIBuffer_Instancing::Initialize(void* pArg)
{
	return S_OK;
}

HRESULT CVIBuffer_Instancing::Bind_Buffers()
{

	ID3D11Buffer* pVertexBufffers[] = {
		m_pVB,
		m_pVBInstance
	};

	_uint				iVertexStrides[] = {
		m_iVertexStride,
		m_iInstanceVertexStride
	};

	_uint				iOffsets[] = {
		0,
		0
	};

	m_pContext->IASetVertexBuffers(0, m_iNumVertexBuffers, pVertexBufffers, iVertexStrides, iOffsets);
	m_pContext->IASetIndexBuffer(m_pIB, m_eIndexFormat, 0);
	m_pContext->IASetPrimitiveTopology(m_ePrimitive_Topology);

	return S_OK;
}

HRESULT CVIBuffer_Instancing::Render()
{
	m_pContext->DrawIndexedInstanced(m_iNumIndexPerInstance, m_iNumInstance, 0, 0, 0);

	return S_OK;
}


_float CVIBuffer_Instancing::Get_RandomNormalize()
{
	return (_float)rand() / RAND_MAX;
}

_float CVIBuffer_Instancing::Get_Random(_float fMin, _float fMax)
{
	return fMin + (fMax - fMin) * Get_RandomNormalize();
}

void CVIBuffer_Instancing::Free()
{
	__super::Free();

	Safe_Release(m_pVBInstance);

}
