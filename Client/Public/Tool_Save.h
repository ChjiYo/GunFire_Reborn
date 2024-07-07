#pragma once

#include "Tool.h"

BEGIN(Client)

class CTool_Save final : public CTool
{
private:
	CTool_Save(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CTool_Save() = default;

public:
	virtual HRESULT Initialize();
	virtual HRESULT Render();

public:
	HRESULT Save_Button();

public:
	HRESULT Save_Button_Pressed(bool* bShowSaveSuccessMessage, bool* bShowSaveFailMessage);

private:
	bool bShowSettings = { true };
	bool bShowSaveSuccessMessage = { false };
	bool bShowSaveFailMessage = { false };

public:
	static CTool_Save* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;
};

END

