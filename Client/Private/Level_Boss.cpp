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

	if (FAILED(Ready_Layer_BackGround(TEXT("Layer_BackGround"))))
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

HRESULT CLevel_Boss::Ready_Layer_BackGround(const _wstring & strLayerTag)
{
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_BOSS, TEXT("Prototype_GameObject_Terrain_Boss"), strLayerTag)))
		return E_FAIL;
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
