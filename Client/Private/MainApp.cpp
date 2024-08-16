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

#include "UI_Mon_BarPanel.h"
#include "UI_Mon_HpBar.h"
#include "UI_Mon_HpBlankBar.h"

#include "UI_Weapon_BBullet.h"
#include "UI_Weapon_GBullet.h"
#include "UI_Weapon_YBullet.h"
#include "UI_Weapon_Panel.h"
#include "UI_Weapon_NumPanel.h"

#include "UI_Skill_Panel.h"
#include "UI_Skill_Dash.h"
#include "UI_Skill_Aiming.h"

CMainApp::CMainApp()
	: m_pGameInstance { CGameInstance::Get_Instance() }
{
	Safe_AddRef(m_pGameInstance);
}

HRESULT CMainApp::Initialize()
{
	srand((unsigned int)time(NULL));

	if (FAILED(m_pGameInstance->Initialize_Engine(g_hInst, g_hWnd, true, LEVEL_END, g_iWinSizeX, g_iWinSizeY, &m_pDevice, &m_pContext)))
		return E_FAIL;

	if (FAILED(Ready_Prototype_Component_ForStatic()))
		return E_FAIL;
	
	if (FAILED(Ready_Prototoype_Resource_ForStatic()))
		return E_FAIL;

	if (FAILED(Ready_Gara()))
		return E_FAIL;

	//LOGO 먼저 (실수 2스택!!)
	if (FAILED(Open_Level(LEVEL_LOGO)))
		return E_FAIL;	

	if (FAILED(m_pGameInstance->Add_Font(m_pDevice, m_pContext,TEXT("Font_Nexon"), TEXT("../Bin/Resources/Fonts/144ex.spriteFont"))))
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
		pCamera->Priority_Update(fTimeDelta);

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

	/* For.Prototype_Component_Texture_UI_Player_BarPanel*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_Player_BarPanel"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/GunFire_Resources/Textures/UI/Bar/UI_Player_Panel.png")))))
		return E_FAIL;
	  
	/* For.Prototype_Component_Texture_UI_Player_Icon */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_Player_Icon"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/GunFire_Resources/Textures/UI/Bar/UI_Player_Icon.png")))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_UI_Player_Bar */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_Player_Bar"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/GunFire_Resources/Textures/UI/Bar/UI_Player_Bar.png")))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_UI_Player_HpBar */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_Player_HpBar"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/GunFire_Resources/Textures/UI/Bar/UI_Player_HpBar.png")))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_UI_Player_ShieldBar */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_Player_ShieldBar"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/GunFire_Resources/Textures/UI/Bar/UI_Player_ShieldBar.png")))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_UI_Player_HpBlankBar */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_Player_HpBlankBar"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/GunFire_Resources/Textures/UI/Bar/UI_Player_HpBlankBar.png")))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_UI_Cross_Hair */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_Cross_Hair"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/GunFire_Resources/Textures/UI/CrossHair/CrossHair.png")))))
		return E_FAIL;

	// UI_CrossHair 

		/* For.Prototype_Component_Texture_UI_Player_ShieldBlankBar */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_Player_ShieldBlankBar"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/GunFire_Resources/Textures/UI/Bar/UI_Player_ShieldBlankBar.png")))))
		return E_FAIL;

	//UI_Monster_Bar
	
	/* For.Prototype_Component_Texture_UI_Monster_BarPanel */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_Monster_BarPanel"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/GunFire_Resources/Textures/UI/Bar/UI_Monster_Panel.png")))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_UI_Monster_HpBar */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_Monster_HpBar"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/GunFire_Resources/Textures/UI/Bar/UI_Monster_HpBar.png")))))
		return E_FAIL;

	/* For.Prototype_Component_Texture_UI_Monster_HpBlankBar */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_Monster_HpBlankBar"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/GunFire_Resources/Textures/UI/Bar/UI_Monster_HpBlankBar.png")))))
		return E_FAIL;

	//Weapon

	//Prototype_Component_Texture_UI_Weapon_Panel
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_Weapon_Panel"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/GunFire_Resources/Textures/UI/Weapon/UI_Weapon_Panel.png")))))
		return E_FAIL;

	//Prototype_Component_Texture_UI_Weapon_NumPanel
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_Weapon_NumPanel"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/GunFire_Resources/Textures/UI/Weapon/UI_Weapon_NumPanel.png")))))
		return E_FAIL;

	//Prototype_Component_Texture_UI_Weapon_BlueBullet
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_Weapon_BlueBullet"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/GunFire_Resources/Textures/UI/Weapon/UI_Weapon_BlueBullet.png")))))
		return E_FAIL;

	//Prototype_Component_Texture_UI_Weapon_GreenBullet
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_Weapon_GreenBullet"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/GunFire_Resources/Textures/UI/Weapon/UI_Weapon_GreenBullet.png")))))
		return E_FAIL;

	//Prototype_Component_Texture_UI_Weapon_YellowBullet
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_Weapon_YellowBullet"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/GunFire_Resources/Textures/UI/Weapon/UI_Weapon_YellowBullet.png")))))
		return E_FAIL;

	//Prototype_Component_Texture_UI_Weapon_Icon

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_Weapon_Icon"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/GunFire_Resources/Textures/UI/Weapon/UI_Weapon_Icon_%d.png"),4))))
		return E_FAIL;

	//Skill

	//Prototype_Component_Texture_UI_Skill_Panel
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_Skill_Panel"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/GunFire_Resources/Textures/UI/Skill/UI_Skill_Panel.png")))))
		return E_FAIL;

	//Prototype_Component_Texture_UI_Skill_Aiming
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_Skill_Aiming"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/GunFire_Resources/Textures/UI/Skill/UI_Skill_Dash.png")))))
		return E_FAIL;

	//Prototype_Component_Texture_UI_Skill_Dash
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_Skill_Dash"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/GunFire_Resources/Textures/UI/Skill/UI_Skill_Dash.png")))))
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

	/* For.Prototype_GameObject_UI_CrossHair */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_Cross_Hair"),
		CUI_Cross_Hair::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	// UI_Monster_State

	/* For.Prototype_GameObject_UI_Monster_BarPanel */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_Monster_BarPanel"),
		CUI_Mon_BarPanel::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_UI_Monster_HpBar */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_Monster_HpBar"),
		CUI_Mon_HpBar::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_UI_Monster_HpBlankBar */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_Monster_HpBlankBar"),
		CUI_Mon_HpBlankBar::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	//UI_Weapon_State

		/* For.Prototype_GameObject_UI_Weapon_Panel */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_Weapon_Panel"),
		CUI_Weapon_Panel::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_UI_Weapon_NumPanel */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_Weapon_NumPanel"),
		CUI_Weapon_NumPanel::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_UI_Weapon_BlueBullet */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_Weapon_BlueBullet"),
		CUI_Weapon_BBullet::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_UI_Weapon_GreenBullet */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_Weapon_GreenBullet"),
		CUI_Weapon_GBullet::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_UI_Weapon_YellowBullet */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_Weapon_YellowBullet"),
		CUI_Weapon_YBullet::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_UI_Weapon_Icon */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_Weapon_Icon"),
		CUI_Weapon_YBullet::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	//UI_Skill_State

	/* For.Prototype_GameObject_UI_SKill_Panel */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_SKill_Panel"),
		CUI_Skill_Panel::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_UI_SKill_Dash */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_SKill_Dash"),
		CUI_Skill_Dash::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_GameObject_UI_SKill_Aiming */
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_SKill_Aiming"),
		CUI_Skill_Aiming::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	return S_OK;
}

HRESULT CMainApp::Ready_Gara()
{
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


	Safe_Release(m_pContext);
	Safe_Release(m_pDevice);

	m_pGameInstance->Release_Engine();

	Safe_Release(m_pGameInstance);	

	__super::Free();
}

