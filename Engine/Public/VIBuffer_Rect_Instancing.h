#pragma once

#include "VIBuffer_Instancing.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_Rect_Instancing final : public CVIBuffer_Instancing
{
private:
	CVIBuffer_Rect_Instancing(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CVIBuffer_Rect_Instancing(const CVIBuffer_Rect_Instancing& Prototype);
	virtual ~CVIBuffer_Rect_Instancing() = default;

public:
	virtual HRESULT Initialize_Prototype(const CVIBuffer_Instancing::VIBUFFER_INSTANCE_DESC* pInitialDesc);
	virtual HRESULT Initialize(void* pArg) override;

public:
	static CVIBuffer_Rect_Instancing* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const CVIBuffer_Instancing::VIBUFFER_INSTANCE_DESC* pInitialDesc);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END