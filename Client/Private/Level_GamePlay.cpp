#include "stdafx.h"
#include "..\Public\Level_GamePlay.h"

#include "GameInstance.h"
#include "Camera_Free.h"
#include "Element.h"

#include "UI_Cross_Hair.h"
#include "Player.h"
#include "Weapon_Player_Pistol.h"

#include "Mon_Archer.h"

CLevel_GamePlay::CLevel_GamePlay(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CLevel{ pDevice, pContext } 
{
}

HRESULT CLevel_GamePlay::Initialize()
{
	m_iLevelIndex = LEVEL_GAMEPLAY;

	if (FAILED(Ready_Lights()))
		return E_FAIL;

	if (FAILED(Ready_Layer_BackGround(TEXT("Layer_BackGround"))))
		return E_FAIL;

	if (FAILED(Ready_LandObjects()))
		return E_FAIL;

	if (FAILED(Ready_Layer_Camera(TEXT("Layer_Camera"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Effect(TEXT("Layer_Effect"))))
		return E_FAIL;
	
	if (FAILED(Ready_UI_PlayerState()))
		return E_FAIL;

	if (FAILED(Ready_UI_WeaponState()))
		return E_FAIL;

	//if (FAILED(Ready_UI_SkillState()))
	//	return E_FAIL;
	
	if (FAILED(Ready_CrossHair()))
		return E_FAIL;

	if (FAILED(Ready_Layer_DecoObject(TEXT("Layer_Deco"))))
		return E_FAIL;

	if (FAILED(ParseInitialize()))
		return E_FAIL;

	if (FAILED(Ready_Layer_Item(TEXT("Layer_Item"))))
		return E_FAIL;

	

	return S_OK;
}

void CLevel_GamePlay::Update(_float fTimeDelta)
{
}

HRESULT CLevel_GamePlay::Render()
{
	SetWindowText(g_hWnd, TEXT("게임플레이레벨"));

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Lights()
{
	LIGHT_DESC			LightDesc{};

	LightDesc.eType = LIGHT_DESC::TYPE_DIRECTIONAL;
	LightDesc.vDirection = _float4(1.f, -1.f, 1.f, 0.f);
	LightDesc.vDiffuse = _float4(1.f, 1.f, 1.f, 1.f);
	LightDesc.vAmbient = _float4(1.f, 1.f, 1.f, 1.f);
	LightDesc.vSpecular = _float4(0.f, 1.f, 0.f, 1.f);

	if (FAILED(m_pGameInstance->Add_Light(LightDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_BackGround(const _wstring & strLayerTag)
{
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Terrain"), strLayerTag)))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Sky"), strLayerTag)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Camera(const _wstring & strLayerTag)
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

	CameraDesc.pPlayerTransform = dynamic_cast<CTransform*>(m_pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_Player"), TEXT("Com_Transform")));

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Camera_Free"), strLayerTag, &CameraDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Effect(const _wstring & strLayerTag)
{
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Effect_Deco_Snow"), strLayerTag)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_LandObjects()
{
	if (FAILED(Ready_Layer_Player(TEXT("Layer_Player"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Monster(TEXT("Layer_Monster"))))
		return E_FAIL;

	return S_OK;
}
HRESULT CLevel_GamePlay::Ready_Layer_Item(const wstring& strLayerTag)
{
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Item_Sniper"), strLayerTag)))
		return E_FAIL;

	return S_OK;
}
HRESULT CLevel_GamePlay::Ready_UI_PlayerState()
{
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_Player_BarPanel"), TEXT("Layer_UI_Player_ABarPanel"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_Player_Icon"), TEXT("Layer_UI_Player_Icon"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_Player_HpBar"), TEXT("Layer_UI_Player_UI_Player_HpBar"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_Player_ShieldBar"), TEXT("Layer_UI_Player_UI_Player_ShieldBar"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_Player_HpBlankBar"), TEXT("Layer_UI_Player_BHpBlankBar"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_Player_ShieldBlankBar"), TEXT("Layer_UI_Player_BShieldBlankBar"))))
		return E_FAIL;

	return S_OK;
}
HRESULT CLevel_GamePlay::Ready_UI_WeaponState()
{
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_Weapon_Panel"), TEXT("Layer_UI_AWeapon_Panel"))))
		return E_FAIL;
	
	CUIObject::UI_DESC		UIDesc = {};
	
	for (int i = 0; i < 3; ++i)
	{
		UIDesc.iNumUI = i;
		if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_Weapon_NumPanel"), TEXT("Layer_UI_AWeapon_NumPanel"), &UIDesc)))
			return E_FAIL;
	}

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_Weapon_BlueBullet"), TEXT("Layer_UI_Weapon_BlueBullet"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_Weapon_GreenBullet"), TEXT("Layer_UI_Weapon_GreenBullet"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_Weapon_YellowBullet"), TEXT("Layer_UI_Weapon_YellowBullet"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_Weapon_Icon"), TEXT("Layer_UI_Weapon_Icon"))))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_UI_SkillState()
{
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_SKill_Panel"), TEXT("Layer_UI_Weapon_SKill_Panel"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_SKill_Dash"), TEXT("Layer_UI_Weapon_SKill_Dash"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_SKill_Aiming"), TEXT("Layer_UI_Weapon_SKill_Aiming"))))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_CrossHair()
{
	CUI_Cross_Hair::CROSS_HAIR_DESC Desc = {  };
	Desc.fSpeedPerSec = 10.f;

	for (int i = 0; i < 4; i++)
	{
		Desc.iNum = i;
		if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_Cross_Hair"), TEXT("Layer_UI_CrossHair") , &Desc)))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Player(const wstring& strLayerTag)
{
	CPlayer::PLAYER_DESC	Desc = {};
	Desc.iHp = 65;
	Desc.iMaxHp = 65;
	Desc.iShield = 65;
	Desc.iMaxShield = 65;
	Desc.fSpeedPerSec = 5.f;
	Desc.fRotationPerSec = XMConvertToRadians(90.0f);

 	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Player"), strLayerTag, &Desc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Monster(const _wstring & strLayerTag)
{
	CMonster::MON_DESC	Desc = {};
	Desc.iMaxHp = 30.f;
	Desc.iHp = 30.f;

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Mon_Archer"), strLayerTag, &Desc)))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Mon_Bug"), strLayerTag, &Desc)))
		return E_FAIL;

	/*if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Mon_Knight"), strLayerTag, &Desc)))
		return E_FAIL;*/

	/*if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Mon_Spear"), strLayerTag, &Desc)))
		return E_FAIL; */

	/*if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Mon_Deash_Boom"), strLayerTag, &Desc)))
		return E_FAIL; */

	/*if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Mon_Shield_Knight"), strLayerTag, &Desc)))
		return E_FAIL; */

return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_DecoObject(const _wstring& strLayerTag)
{

	CElement::DECO_DESC Desc = {};

	_uint m_iNumDeco = { 1 };

	for (int i = 0; i < m_iNumDeco; ++i)
	{
		Desc.iNumDeco = i;

		if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Door"), strLayerTag , &Desc)))
			return E_FAIL;
	}

	return S_OK;
}

CLevel_GamePlay * CLevel_GamePlay::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CLevel_GamePlay*		pInstance = new CLevel_GamePlay(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX(TEXT("Failed to Created : CLevel_GamePlay"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_GamePlay::Free()
{
	__super::Free();
}
