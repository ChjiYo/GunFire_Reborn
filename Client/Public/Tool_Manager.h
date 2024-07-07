#pragma once

#include "Tool.h"
#include "Tool_Object.h"
#include "Tool_Save.h"

BEGIN(Client)

class CTool_Manager final : public CTool
{
private:
	CTool_Manager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CTool_Manager() = default;

public:
	HRESULT Initialize();
	HRESULT Tool_Render();
	HRESULT Tool_RenderEnd();
public:
	HRESULT Ready_Init();

private:
	CTool_Object*	m_pTool_Object = { nullptr };
	CTool_Save*		m_pTool_Save = { nullptr };

public:
	static CTool_Manager* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;
};

END

