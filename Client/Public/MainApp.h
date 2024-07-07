#pragma once

#include "Client_Defines.h"
#include "Base.h"
#include "Tool_Manager.h"

BEGIN(Engine)
class CGameInstance;
END

BEGIN(Client)

class CMainApp final : public CBase
{
private:
	CMainApp();
	virtual ~CMainApp() = default;

public:
	HRESULT Initialize();
	void Update(_float fTimeDelta);
	HRESULT Render();

private:
	ID3D11Device*				m_pDevice = { nullptr };
	ID3D11DeviceContext*		m_pContext = { nullptr };
	CGameInstance*				m_pGameInstance = { nullptr };
	CTool_Manager*				m_pTool_Manager = { nullptr };

	_bool m_bShowTool = { false };

private:
	HRESULT Ready_Prototype_Component_ForStatic();
	HRESULT Ready_Prototoype_Resource_ForStatic();
	HRESULT Open_Level(LEVELID eStartLevelID);

public:
	static CMainApp* Create();
	virtual void Free() override;
};

END