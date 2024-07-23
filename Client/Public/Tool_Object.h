#pragma once

#include "Tool.h"

BEGIN(Client)

class CTool_Object final : public CTool
{
private:
	CTool_Object(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CTool_Object() = default;

public:
	virtual HRESULT Initialize();
	virtual HRESULT Render();

public:
	HRESULT Level_Tab();
	HRESULT Prototype_ObjectList_Tab();
	HRESULT Layer_ObjectList_Tab();
	HRESULT Setting_Tab();

private:
	HRESULT ShowLevels();
	HRESULT Show_PrototypeObjects();
	HRESULT Show_LayerObjects();
	HRESULT Click_Camera_FreeView(_bool bColliderToggle);

private:
	HRESULT SpawnObjectAtZero(const std::string& type);
	HRESULT Click_Button_Release(std::vector<class CGameObject*>& selectedGameObjects);
	HRESULT CopySelectedObject(const wstring& layerName, CGameObject* sourceObject);
	HRESULT PickingSelectedObject(const wstring& layerName, CGameObject* sourceObject,_float3 vPos);
	_float3  PickingPos();

private:
	_bool m_bMovementEnabled = { false };
	_bool m_bPickingCheckBox = { false };
	_float3 averageRotation = {};
	_float3 m_vPickingPos = {};
	_uint m_iNumNaviPicking = {0};
	_bool m_bNaviPicking = {};

public:
	static CTool_Object* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;
};

END

