#pragma once

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_Instancing abstract : public CVIBuffer
{
protected:
	CVIBuffer_Instancing(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CVIBuffer_Instancing(const CVIBuffer_Instancing& Prototype);
	virtual ~CVIBuffer_Instancing() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual HRESULT Bind_Buffers() override;
	virtual HRESULT Render() override;

protected:
	/* 모델(Instance하나)의 상태제어를 위한 행렬정보를 가지는 정점 버퍼.  */
	ID3D11Buffer* m_pVBInstance = { nullptr };

	/* 위의 정점버퍼에 추가된 정점 하나의 크기. */
	_uint					m_iInstanceVertexStride = { 0 };

	/* 인스턴싱을 통해 몇개의 모델(인스턴스)를 그려낼것이다. */
	_uint					m_iNumInstance = { 0 };

	_uint					m_iNumIndexPerInstance = { 0 };

protected:
	_float Get_RandomNormalize();
	_float Get_Random(_float fMin, _float fMax);

public:
	virtual CComponent* Clone(void* pArg) = 0;
	virtual void Free() override;
};

END