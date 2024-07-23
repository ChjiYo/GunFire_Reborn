#include "stdafx.h"

#include "Tool_Save.h"
#include "GameObject.h"

#include "GameInstance.h"
#include "Terrain.h"

CTool_Save::CTool_Save(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CTool{ pDevice , pContext }
{
}

HRESULT CTool_Save::Initialize()
{
	return S_OK;
}

HRESULT CTool_Save::Render()
{
	ImGui::Begin("Settings", &bShowSettings); // "Settings" 창 시작

	Save_Button();

	if (bShowSaveSuccessMessage) {
		ImGui::Text("File saved successfully!");
	}

	if (bShowSaveFailMessage) {
		ImGui::Text("Failed to save file!");
	}


	ImGui::End(); // "Settings" 창 종료

	return S_OK;
}

HRESULT CTool_Save::Save_Button()
{
	if (ImGui::Button("Save")) { // "Save" 버튼
		Save_Button_Pressed(&bShowSaveSuccessMessage, &bShowSaveFailMessage);
	}

	if (ImGui::Button("NaviSave")) { // "Save" 버튼
		Save_Navi();
	}

	return S_OK;
}

HRESULT CTool_Save::Save_Navi()
{
	_uint currentLevel = m_pGameInstance->GetCurrentLevelIndex();

	CTerrain* pTerrain = dynamic_cast<CTerrain*>(m_pGameInstance->Get_Layer(currentLevel, TEXT("Layer_BackGround")).back());
	CNavigation* TerrainNavi = dynamic_cast<CNavigation*>(m_pGameInstance->Get_Component(currentLevel, TEXT("Layer_BackGround"), TEXT("Com_Navigation")));
	TerrainNavi->Save_Navi();

	return S_OK;
}

HRESULT CTool_Save::Save_Button_Pressed(bool* bShowSaveSuccessMessage, bool* bShowSaveFailMessage)
{
	// 오브젝트마다의 정보
	vector<FILEDATA> vecFileData;

	// 레벨 정보 
	_uint currentLevel = m_pGameInstance->GetCurrentLevelIndex();

	vector<pair<wstring, list<CGameObject*>>> objectLayersVector;
	m_pGameInstance->AddObjectLayersVector(currentLevel, &objectLayersVector);

	// 여기에 스킵할 레이어 이름을 정의
	unordered_set<wstring> skipLayers =
	{ L"Layer_BackGround", L"Layer_Camera",L"Layer_Monster" ,L"Layer_Player",L"Layer_Skill_Player_Bullet"};

	// "Layer_UI"로 시작하는 모든 레이어를 스킵할 레이어에 추가
	for (const auto& object : objectLayersVector)
	{
		if (object.first.find(L"Layer_UI") == 0 ||
			object.first.find(L"Layer_ZUI") == 0 ||
			object.first.find(L"Layer_XUI") == 0 ||
			object.first.find(L"Layer_Npc") == 0)
		{
			skipLayers.insert(object.first);
		}
	}

	for (auto& object : objectLayersVector)
	{
		// 현재 레이어가 스킵할 리스트에 있는지 검사
		if (skipLayers.find(object.first) != skipLayers.end()) {
			continue;  // 스킵할 레이어면 다음 반복으로 넘어감
		}

		for (auto& iter : object.second)
		{
			CTransform* transform = dynamic_cast<CTransform*>(iter->Get_Component(TEXT("Com_Transform")));

			// Layer_ 뒤의 문자열 추출
			wstring layerName = object.first;
			size_t pos = layerName.find(L"Layer_");
			if (pos != wstring::npos) {
				// "Layer_" 다음 문자열 시작 위치는 pos + 6
				wstring suffix = layerName.substr(pos + 6);

				// 새로운 prefix 생성
				wstring newPrefix = L"Prototype_GameObject_" + suffix;

				// 이 새로운 문자열을 vecFileData에 추가
				FILEDATA tData = { newPrefix, object.first, currentLevel, transform->Get_State(CTransform::STATE_POSITION), transform->Get_Scaled(), true };
				vecFileData.emplace_back(tData);
			}
		}
	}

	// currentLevel에 따라 파일 이름 지정
	wstring filePath;
	switch (currentLevel)
	{
	case LEVEL_STATIC:
		filePath = L"../Bin/LevelStaticObjects.txt";
		break;
	case LEVEL_LOADING:
		filePath = L"../Bin/LevelLoadingObjects.txt";
		break;

	case LEVEL_GAMEPLAY:
		filePath = L"../Bin/LevelGamePlayObjects.txt";
		break;

	default:
		filePath = L"../Bin/DefaultLevelObjects.txt"; // default 이름
		break;
	}

	// 이걸 레벨당 사본 객체 리스트를 넘겨줘야 한다.
	HRESULT result = m_pGameInstance->SaveObjects(filePath.c_str(), &vecFileData);
	if (result == S_OK) {
		*bShowSaveSuccessMessage = true;
		*bShowSaveFailMessage = false;

		return S_OK;
	}
	else {
		*bShowSaveFailMessage = true;
		*bShowSaveSuccessMessage = false;

		return E_FAIL;
	}

	return S_OK;
}

CTool_Save* CTool_Save::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CTool_Save* pInstance = new CTool_Save(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX(TEXT("Failed To Created : CTool_Save"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTool_Save::Free()
{
	__super::Free();
}
