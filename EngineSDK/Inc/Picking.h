#pragma once

#include "Base.h"

BEGIN(Engine)

class CPicking final : public CBase
{
private:
	CPicking(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CPicking() = default;

public:
	HRESULT Initialize(HWND hWnd);
	void Update();
	void Transform_ToLocalSpace(const XMMATRIX* pWorldMatrix);
	_bool Picked_InLocalSpace(const _vector* pPointA, const _vector* pPointB, const _vector* pPointC,_vector* pPickPos);
	_bool Picked_InWorldSpace(_vector pPointA, _vector pPointB, _vector pPointC, _vector* pPickPos);

private:
	ID3D11Device*			m_pDevice = { nullptr };
	ID3D11DeviceContext*	m_pContext = { nullptr };

	class CGameInstance* m_pGameInstance = { nullptr };

	HWND						m_hWnd = {};
	_float3						m_vRayDir = {};
	_float3						m_vRayPos = {};

	_float3						m_vRayDir_Local = {};
	_float3						m_vRayPos_Local = {};

public:
	static CPicking* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, HWND hWnd);
	virtual void Free() override;
};

END