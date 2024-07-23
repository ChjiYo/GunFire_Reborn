#include "stdafx.h"

#include "Tool_Manager.h"

#include "..\Public\MainApp.h"

#include "GameInstance.h"
#include "Level_Loading.h"

#include "Camera_Free.h"

#include "UI_Player_BarPanel.h"
#include "UI_Player_Icon.h"
#include "UI_Player_Bar.h"
#include  "UI_Player_HpBlankBar.h"
#include  "UI_Player_ShieldBlankBar.h"
#include  "UI_Player_HpBar.h"
#include  "UI_Player_ShieldBar.h"

#include "UI_Cross_Hair.h"

CMainApp::CMainApp()
	: m_pGameInstance { CGameInstance::Get_Instance() }
{
	Safe_AddRef(m_pGameInstance);
}

HRESULT CMainApp::Initialize()
{
	if (FAILED(m_pGameInstance->Initialize_Engine(g_hInst, g_hWnd, true, LEVEL_END, g_iWinSizeX, g_iWinSizeY, &m_pDevice, &m_pContext)))
		return E_FAIL;

	if (FAILED(Ready_Prototype_Component_ForStatic()))
		return E_FAIL;

	if (FAILED(Ready_Prototoype_Resource_ForStatic()))
		return E_FAIL;

	/*if (FAILED(Ready_Gara()))
		return E_FAIL;*/

	//LOGO 먼저 (실수 2스택!!)
	if (FAILED(Open_Level(LEVEL_LOGO)))
		return E_FAIL;	

	m_pTool_Manager = CTool_Manager::Create(m_pDevice, m_pContext);

	return S_OK;
}

void CMainApp::Update(_float fTimeDelta)
{
	if (m_pGameInstance->Key_Down('O'))
		m_bShowTool ? m_bShowTool = false : (m_bShowTool = true );

	if(m_pGameInstance->Key_Down('P'))
		m_bShowCursor ? m_bShowCursor = false : (m_bShowCursor = true);

	if (m_bShowTool)
	{
		CCamera_Free* pCamera = dynamic_cast<CCamera_Free*>(m_pGameInstance->Get_Layer(LEVEL_GAMEPLAY, TEXT("Layer_Camera")).back());
		pCamera->Update(fTimeDelta);

		fTimeDelta = 0.f;
	}

	ShowCursor(m_bShowCursor);

	m_pGameInstance->Update_Engine(fTimeDelta);
}

HRESULT CMainApp::Render()
{
	if (m_bShowTool)
		m_pTool_Manager->Tool_Render();

	m_pGameInstance->Clear_BackBuffer_View(_float4(0.f, 0.f, 1.f, 1.f));
	m_pGameInstance->Clear_DepthStencil_View();

	m_pGameInstance->Render_Engine();

	if (m_bShowTool)
		m_pTool_Manager->Tool_RenderEnd();

	m_pGameInstance->Present();

	return S_OK;
}

HRESULT CMainApp::Ready_Prototype_Component_ForStatic()
{
	/* For.Prototype_Component_VIBuffer_Rect */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"),
		CVIBuffer_Rect::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxPosTex */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxPosTex"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxPosTex.hlsl"), VTXPOSTEX::Elements, VTXPOSTEX::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_VIBuffer_Rect */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Cube"),
		CVIBuffer_Cube::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	return S_OK;
}

HRESULT CMainApp::Ready_Prototoype_Resource_ForStatic()
{
	/* Texture Load */

	// UI_Player_Bar 

	/* For.Prototype_Component_Player_BarPanel*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_Player_BarPanel"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/GunFire_Resources/Textures/UI/Bar/UI_Player_Panel.png")))))
		return E_FAIL;

	/* For.Prototype_Component_Player_Icon */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_Player_Icon"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/GunFire_Resources/Textures/UI/Bar/UI_Player_Icon.png")))))
		return E_FAIL;

	/* For.Prototype_Component_Player_Bar */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_Player_Bar"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/GunFire_Resources/Textures/UI/Bar/UI_Player_Bar.png")))))
		return E_FAIL;

	/* For.Prototype_Component_Player_HpBar */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_Player_HpBar"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/GunFire_Resources/Textures/UI/Bar/UI_Player_HpBar.png")))))
		return E_FAIL;

	/* For.Prototype_Component_Player_ShieldBar */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_Player_ShieldBar"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/GunFire_Resources/Textures/UI/Bar/UI_Player_ShieldBar.png")))))
		return E_FAIL;

	/* For.Prototype_Component_Player_HpBar */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_Player_HpBlankBar"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/GunFire_Resources/Textures/UI/Bar/UI_Player_HpBlankBar.png")))))
		return E_FAIL;

	/* For.Prototype_Component_Player_ShieldBar */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_Cross_Hair"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/GunFire_Resources/Textures/UI/CrossHair/CrossHair.png")))))
		return E_FAIL;

	// UI_CrossHair 

		/* For.Prototype_Component_Cross_Hair */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_Player_ShieldBlankBar"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/GunFire_Resources/Textures/UI/Bar/UI_Player_ShieldBlankBar.png")))))
		return E_FAIL;



	/* Prototype Load */

	// UI_Player_State

	/* For.Prototype_GameObject_UI_Player_BarPanel */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_Player_BarPanel"),
		CUI_Player_BarPanel::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_UI_Player_Icon */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_Player_Icon"),
		CUI_Player_Icon::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_UI_Player_Bar */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_Player_Bar"),
		CUI_Player_Bar::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_UI_Player_HpBar */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_Player_HpBar"),
		CUI_Player_HpBar::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_UI_Player_ShieldBar */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_Player_ShieldBar"),
		CUI_Player_ShieldBar::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_UI_Player_HpBlankBar */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_Player_HpBlankBar"),
		CUI_Player_HpBlankBar::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_UI_Player_ShieldBlankBar */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_Player_ShieldBlankBar"),
		CUI_Player_ShiieldBlankBar::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	//CrossHair

	/* For.Prototype_GameObject_UI_CrossHair */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_Cross_Hair"),
		CUI_Cross_Hair::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	return S_OK;
}

HRESULT CMainApp::Ready_Gara()
{
	_ulong			dwByte = { 0 };

	HANDLE			hFile = CreateFile(TEXT("../Bin/DataFiles/Navigation.dat"), GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
	if (0 == hFile)
		return E_FAIL;

	_float3			vPoints[3];

	vPoints[0] = _float3(0.0f, 0.f, 0.f);
	vPoints[1] = _float3(0.0f, 0.f, 10.f);
	vPoints[2] = _float3(10.0f, 0.f, 0.f);
	WriteFile(hFile, vPoints, sizeof(_float3) * 3, &dwByte, nullptr);

	vPoints[0] = _float3(0.0f, 0.f, 10.f);
	vPoints[1] = _float3(10.0f, 0.f, 10.f);
	vPoints[2] = _float3(10.0f, 0.f, 0.f);
	WriteFile(hFile, vPoints, sizeof(_float3) * 3, &dwByte, nullptr);

	vPoints[0] = _float3(0.0f, 0.f, 20.f);
	vPoints[1] = _float3(10.0f, 0.f, 10.f);
	vPoints[2] = _float3(0.0f, 0.f, 10.f);
	WriteFile(hFile, vPoints, sizeof(_float3) * 3, &dwByte, nullptr);

	vPoints[0] = _float3(10.0f, 0.f, 10.f);
	vPoints[1] = _float3(20.0f, 10.f, 0.f);
	vPoints[2] = _float3(10.0f, 0.f, 0.f);
	WriteFile(hFile, vPoints, sizeof(_float3) * 3, &dwByte, nullptr);

	CloseHandle(hFile);

	return S_OK;
}

HRESULT CMainApp::Open_Level(LEVELID eStartLevelID)
{
	if (FAILED(m_pGameInstance->Change_Level(CLevel_Loading::Create(m_pDevice, m_pContext, eStartLevelID))))
		return E_FAIL;

	return S_OK;
}

CMainApp * CMainApp::Create()
{
	CMainApp*		pInstance = new CMainApp();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX(TEXT("Failed to Created : CMainApp"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMainApp::Free()
{
	Safe_Release(m_pTool_Manager);

	__super::Free();

	Safe_Release(m_pContext);
	Safe_Release(m_pDevice);

	m_pGameInstance->Release_Engine();

	Safe_Release(m_pGameInstance);	
}

