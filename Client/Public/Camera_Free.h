#pragma once

#include "Client_Defines.h"
#include "Camera.h"

BEGIN(Engine)

class CState_Machine;
class CNavigation;

END

BEGIN(Client)

class CCamera_Free final : public CCamera
{	
public:
	typedef struct : public CCamera::CAMERA_DESC
	{
		_float		fSensor = {};
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

public:
	void Set_FreeView(_bool bCheck) { m_bCameraMode = bCheck; }
	HRESULT Read_Component();

private:
	CState_Machine*		m_pState = { nullptr };
	CNavigation*		m_pNavigationCom = { nullptr };

	_float				m_fMouseSensor = {};
	_bool				m_bCameraMode = { true };
private:
	void GamePlay_Camera(_float fTimeDelta);
	void FreeView_Camera(_float fTimeDelta);

public:
	static CCamera_Free* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END