#pragma once

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

#include "Base.h"
#include "Client_Defines.h"

#include "GameInstance.h"

BEGIN(Client)

class CTool abstract : public CBase
{
protected:
	CTool(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CTool() = default;

public:
	virtual HRESULT Initialize();
	virtual HRESULT Render();

protected:
	CGameInstance* m_pGameInstance = {};
	ID3D11Device* m_pDevice = {};
	ID3D11DeviceContext* m_pContext = {};

protected:
	_bool m_bShowImGuiWindows = { true };


public:
	virtual void Free() override;
};

END