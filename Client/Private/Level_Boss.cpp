#include "stdafx.h"

#include "..\Public\Level_Boss.h"

#include "GameInstance.h"
#include "Camera_Free.h"
#include "Element.h"

#include "UI_Cross_Hair.h"
#include "Player.h"
#include "Weapon_Player_Pistol.h"

#include "Mon_Archer.h"
#include "Skill.h"

#include "Terrain.h"

#include "Data_Manager.h"

CLevel_Boss::CLevel_Boss(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CLevel{ pDevice, pContext }
	, m_pDataMgr{ CData_Manager::Get_Instance() }
{
}

HRESULT CLevel_Boss::Initialize()
{
	m_iLevelIndex = LEVEL_BOSS;

	m_pGameInstance->Clear_Light();

	if (FAILED(Ready_Lights()))
		return E_FAIL;

	if (FAILED(Ready_Layer_Ground(TEXT("Layer_BackGround"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Player(TEXT("Layer_Player"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Monster(TEXT("Layer_Monster"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Camera(TEXT("Layer_Camera"))))
		return E_FAIL;

	if (FAILED(Ready_UI_PlayerState()))
		return E_FAIL;

	if (FAILED(Ready_UI_WeaponState()))
		return E_FAIL;

	if (FAILED(Ready_MiniMap()))
		return E_FAIL;

	if (FAILED(Ready_Weapon_Bullet()))
		return E_FAIL;

	if (FAILED(Ready_CrossHair()))
		return E_FAIL;

	if (FAILED(ParseInitialize()))
		return E_FAIL;

	if(FAILED(Ready_Layer_Effect(TEXT("Layer_Effect"))))
		return E_FAIL;

	m_pGameInstance->Stop_All_Sounds(LEVEL_GAMEPLAY);
	m_pGameInstance->Play_Sound(L"BGM_Boss", LEVEL_STATIC, true);

	return S_OK;
}

void CLevel_Boss::Update(_float fTimeDelta)
{
	POINT pt;
	pt.x = g_iWinSizeX * 0.5f;   
	pt.y = g_iWinSizeY * 0.5f;

	ClientToScreen(g_hWnd, &pt);
	SetCursorPos(pt.x, pt.y);
}

HRESULT CLevel_Boss::Render()
{
	SetWindowText(g_hWnd, TEXT("보스레벨"));

	return S_OK;
}

HRESULT CLevel_Boss::Ready_Lights()
{
	LIGHT_DESC			LightDesc{};

	ZeroMemory(&LightDesc, sizeof(LIGHT_DESC));
	LightDesc.eType = LIGHT_DESC::TYPE_DIRECTIONAL;
	LightDesc.vDirection = _float4(1.f, -1.f, 1.f, 0.f);
	LightDesc.vDiffuse = _float4(0.1f, 0.1f, 0.1f, 1.f);
	LightDesc.vAmbient = _float4(0.7725f * 0.1f, 0.5294f * 0.1f, 0.2980f * 0.1f, 1.f); 
	LightDesc.vSpecular = _float4(0.f, 0.f, 0.f, 1.f);

	if (FAILED(m_pGameInstance->Add_Light(LightDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Boss::Ready_Layer_Ground(const _wstring & strLayerTag)
{
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_BOSS, TEXT("Prototype_GameObject_Terrain_Boss"), strLayerTag)))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_BOSS, TEXT("Prototype_GameObject_Sky_Boss"), strLayerTag)))
		return E_FAIL;
}

HRESULT CLevel_Boss::Ready_Layer_Player(const _wstring& strLayerTag)
{
	CPlayer::PLAYER_DESC	Desc = {};
	Desc = m_pDataMgr->Get_PlayerDesc();

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_BOSS, TEXT("Prototype_GameObject_Player"), strLayerTag, &Desc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Boss::Ready_Layer_Monster(const _wstring& strLayerTag)
{
	CMonster::MON_DESC MonDesc = {};
	MonDesc.iMaxHp = 1000.f;
	MonDesc.iHp = 1000.f;
	MonDesc.iNumParts = CMonster::PART_END;

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_BOSS, TEXT("Prototype_GameObject_Mon_Dragon"), strLayerTag, &MonDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Boss::Ready_Layer_Camera(const _wstring& strLayerTag)
{
	CCamera_Free::CAMERA_FREE_DESC			CameraDesc{};

	CameraDesc.vEye = _float3(0.f, 2.f, -10.f);
	CameraDesc.vAt = _float3(0.f, 0.f, 0.f);
	CameraDesc.fFovy = XMConvertToRadians(60.0f); 
	CameraDesc.fNear = 0.1f;
	CameraDesc.fFar = 1000.f;
	CameraDesc.fSpeedPerSec = 5.f;
	CameraDesc.fRotationPerSec = XMConvertToRadians(90.0f);

	CameraDesc.fSensor = 0.1f;

	CameraDesc.pPlayerTransform = dynamic_cast<CTransform*>(m_pGameInstance->Get_Component(LEVEL_BOSS, TEXT("Layer_Player"), TEXT("Com_Transform")));


	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_BOSS, TEXT("Prototype_GameObject_Camera_Free"), strLayerTag, &CameraDesc)))
		return E_FAIL;
	
	return S_OK;
}

HRESULT CLevel_Boss::Ready_Layer_Effect(const _wstring& strLayerTag)
{
	m_pGameInstance->Add_GameObject_ToLayer(LEVEL_BOSS, TEXT("Prototype_GameObject_Player_Hit"), TEXT("Layer_Effect"));

	return S_OK;
}

HRESULT CLevel_Boss::Ready_Layer_UI_Boss(const _wstring& strLayerTag)
{
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_BOSS, TEXT("Prototype_GameObject_UI_Boss_Icon"), strLayerTag)))
		return E_FAIL;
	
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_BOSS, TEXT("Prototype_GameObject_UI_Boss_HpPanel"), strLayerTag)))
		return E_FAIL;
	
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_BOSS, TEXT("Prototype_GameObject_UI_Boss_HpBar"), strLayerTag)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Boss::Ready_UI_PlayerState()
{
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_BOSS, TEXT("Prototype_GameObject_UI_Player_BarPanel"), TEXT("Layer_UI_Player_ABarPanel"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_BOSS, TEXT("Prototype_GameObject_UI_Player_Icon"), TEXT("Layer_UI_Player_Icon"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_BOSS, TEXT("Prototype_GameObject_UI_Player_HpBar"), TEXT("Layer_UI_Player_UI_Player_HpBar"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_BOSS, TEXT("Prototype_GameObject_UI_Player_ShieldBar"), TEXT("Layer_UI_Player_UI_Player_ShieldBar"))))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Boss::Ready_UI_WeaponState()
{
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_BOSS, TEXT("Prototype_GameObject_UI_Weapon_Panel"), TEXT("Layer_UI_AWeapon_Panel"))))
		return E_FAIL;

	CUIObject::UI_DESC		UIDesc = {};

	for (int i = 0; i < 3; ++i)
	{
		UIDesc.iNumUI = i;
		if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_BOSS, TEXT("Prototype_GameObject_UI_Weapon_NumPanel"), TEXT("Layer_UI_AWeapon_NumPanel"), &UIDesc)))
			return E_FAIL;
	}

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_BOSS, TEXT("Prototype_GameObject_UI_Weapon_BlueBullet"), TEXT("Layer_UI_Weapon_BlueBullet"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_BOSS, TEXT("Prototype_GameObject_UI_Weapon_GreenBullet"), TEXT("Layer_UI_Weapon_GreenBullet"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_BOSS, TEXT("Prototype_GameObject_UI_Weapon_YellowBullet"), TEXT("Layer_UI_Weapon_YellowBullet"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_BOSS, TEXT("Prototype_GameObject_UI_Weapon_Icon"), TEXT("Layer_UI_Weapon_Icon"))))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Boss::Ready_UI_SkillState()
{
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_BOSS, TEXT("Prototype_GameObject_UI_SKill_Panel"), TEXT("Layer_UI_Weapon_SKill_Panel"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_BOSS, TEXT("Prototype_GameObject_UI_SKill_Dash"), TEXT("Layer_UI_Weapon_SKill_Dash"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_BOSS, TEXT("Prototype_GameObject_UI_SKill_Aiming"), TEXT("Layer_UI_Weapon_SKill_Aiming"))))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Boss::Ready_MiniMap()
{
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_MiniMap"), TEXT("Layer_UI_MiniMap"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_Player_Mark"), TEXT("Layer_UI_Player_Mark"))))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Boss::Ready_Weapon_Bullet()
{
	CUIObject::UI_DESC  Desc = {};
	for (int i = 0; i < 5; ++i)
	{
		Desc.iNumUI = i;

		m_pGameInstance->Add_GameObject_ToLayer(m_pGameInstance->GetLoadingLevelIndex(), TEXT("Prototype_GameObject_UI_Bullet_Num"), TEXT("Layer_UI_Bullet_Num"), &Desc);
	}

	return S_OK;
}

HRESULT CLevel_Boss::Ready_CrossHair()
{
	CUI_Cross_Hair::CROSS_HAIR_DESC Desc = {  };
	Desc.fSpeedPerSec = 10.f;

	for (int i = 0; i < 4; i++)
	{
		Desc.iNum = i;
		if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_BOSS, TEXT("Prototype_GameObject_UI_Cross_Hair"), TEXT("Layer_UI_CrossHair"), &Desc)))
			return E_FAIL;
	}

	return S_OK;
}


CLevel_Boss* CLevel_Boss::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CLevel_Boss* pInstance = new CLevel_Boss(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX(TEXT("Failed to Created : CLevel_Boss"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_Boss::Free()
{
	__super::Free();
}
