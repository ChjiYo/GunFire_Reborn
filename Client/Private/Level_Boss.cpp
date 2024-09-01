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


CLevel_Boss::CLevel_Boss(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CLevel{ pDevice, pContext }
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


	return S_OK;
}

void CLevel_Boss::Update(_float fTimeDelta)
{

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
	LightDesc.vAmbient = _float4(0.2f, 0.2f, 0.2f, 1.f);
	LightDesc.vSpecular = _float4(1.f, 1.f, 1.f, 1.f);

	if (FAILED(m_pGameInstance->Add_Light(LightDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Boss::Ready_Layer_Ground(const _wstring & strLayerTag)
{
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_BOSS, TEXT("Prototype_GameObject_Terrain_Boss"), strLayerTag)))
		return E_FAIL;
}

HRESULT CLevel_Boss::Ready_Layer_Player(const _wstring& strLayerTag)
{
	CPlayer::PLAYER_DESC	Desc = {};
	Desc.iHp = 65;
	Desc.iMaxHp = 65;
	Desc.iShield = 65;
	Desc.iMaxShield = 65;
	Desc.fSpeedPerSec = 5.f;
	Desc.fRotationPerSec = XMConvertToRadians(90.0f);

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_BOSS, TEXT("Prototype_GameObject_Player"), strLayerTag, &Desc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Boss::Ready_Layer_Monster(const _wstring& strLayerTag)
{
	CMonster::MON_DESC MonDesc = {};
	MonDesc.iMaxHp = 10.f;
	MonDesc.iHp = 10.f;
	
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
