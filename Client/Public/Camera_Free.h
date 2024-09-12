#pragma once

#include "Client_Defines.h"
#include "Camera.h"

BEGIN(Engine)
class CState_Machine;
END

BEGIN(Client)

class CCamera_Free final : public CCamera
{	
public:
	enum CAMERA_TYPE {PLAYER , FREE , SCOPE , CAMERA_END};

public:
	typedef struct : public CCamera::CAMERA_DESC
	{
		_float		fSensor = {};
		CTransform* pPlayerTransform = {};
	}CAMERA_FREE_DESC;

private:
	CCamera_Free(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CCamera_Free(const CCamera_Free& Prototype);
	virtual ~CCamera_Free() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	HRESULT	Ready_Components();

public:
	void Set_CameraType(CAMERA_TYPE eType) { m_eCameraType = eType; }

private:
	CTransform*			m_pPlayerTransform = { nullptr };

	_bool				m_bShowCursor = { true };
	_bool				m_bTrueCurrsor = { false };
	_bool				m_isCurrsorEnable = { true };

	_float				m_fMouseSensor = {};
	_float				m_fScopeLevel = { 0.f };

	CAMERA_TYPE			m_eCameraType = { PLAYER };
	
private:
	void Camera_State(_float fTimeDelta, _float fMul = 1.f);

	void FreeView_Camera(_float fTimeDelta);
	void Position(_float3 vOffSet);
	void Scope_Camera(_float fTimeDelta, _float fMul);

public:
	static CCamera_Free* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END