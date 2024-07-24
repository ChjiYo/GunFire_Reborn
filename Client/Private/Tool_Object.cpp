#include "stdafx.h"

#include "Tool_Object.h"
#include "Level_Loading.h"
#include "GameObject.h"
#include "Camera_Free.h"
#include "Terrain.h"
#include "GameInstance.h"

CTool_Object::CTool_Object(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CTool{ pDevice ,pContext }
{
}

HRESULT CTool_Object::Initialize()
{
	if(FAILED(__super::Initialize()));

	return S_OK;
}

HRESULT CTool_Object::Render()
{
	if (m_bShowImGuiWindows)
	{  
		ImGui::Begin("Main Tab", &m_bShowImGuiWindows);
		if (ImGui::BeginTabBar("MyTabBar"))
		{
			Level_Tab();
			Prototype_ObjectList_Tab();
			Layer_ObjectList_Tab();
			Setting_Tab();
			ImGui::EndTabBar(); 
		}
		ImGui::End();
	}

	return S_OK;
}

HRESULT CTool_Object::Level_Tab()
{
	if (ImGui::BeginTabItem("Levels")) { // "Levels" 탭 아이템 시작
		//레벨 창
		bool bShowLevelWindow = true;
		if (bShowLevelWindow) {
			ShowLevels();
		}
		ImGui::EndTabItem(); // "Levels" 탭 아이템 종료
	}

	return S_OK;
}

HRESULT CTool_Object::Prototype_ObjectList_Tab()
{
	if (ImGui::BeginTabItem("Prototype_ObjectList")) { // "ObjectList" 탭 아이템 시작
		//원형 오브젝트 창
		bool bShowObjectWindow = true;
		if (bShowObjectWindow) {
			Show_PrototypeObjects();
		}
		ImGui::EndTabItem(); // "ObjectList" 탭 아이템 종료
	}

	return S_OK;
}

HRESULT CTool_Object::Layer_ObjectList_Tab()
{
	if (ImGui::BeginTabItem("Layer_ObjectList")) { // "ObjectList" 탭 아이템 시작
		//원형 오브젝트 창
		bool bShowObjectWindow = true;
		if (bShowObjectWindow) {
			Show_LayerObjects();
		}
		ImGui::EndTabItem(); // "ObjectList" 탭 아이템 종료
	}

	return S_OK;
}

HRESULT CTool_Object::Setting_Tab()
{
	_uint currentLevel = m_pGameInstance->GetCurrentLevelIndex();

	if (ImGui::BeginTabItem("Setting")) { // "ObjectList" 탭 아이템 시작
		//원형 오브젝트 창
		bool bShowObjectWindow = true;
		if (bShowObjectWindow) {
			static bool bColliderToggle = true;

			if (ImGui::Checkbox("Camera_FreeView", &bColliderToggle)) {
				Click_Camera_FreeView(bColliderToggle);
			}
		}

		static bool bNaviPicking = false ;

		ImGui::Checkbox("NaviPicking", &bNaviPicking);
		

		CTerrain* pTerrain = dynamic_cast<CTerrain*>(m_pGameInstance->Get_Layer(currentLevel, TEXT("Layer_BackGround")).front());

		if (bNaviPicking)
		{
			if (m_pGameInstance->Key_Down(VK_LBUTTON))
			{
				pTerrain->Add_NaviMesh(PickingPos());
			}
		}
		
		ImGui::EndTabItem(); // "ObjectList" 탭 아이템 종료
	}

	return S_OK;
}

HRESULT CTool_Object::ShowLevels()
{
	// 가정: 레벨 이름과 ID를 가진 배열 또는 리스트
	vector<pair<string, LEVELID>> levels = {
		{"Logo", LEVEL_LOGO},
		{"GamePlay", LEVEL_GAMEPLAY},
	};

	for (auto& level : levels) {
		if (ImGui::Button(level.first.c_str())) {
			return m_pGameInstance->Change_Level(CLevel_Loading::Create(m_pDevice, m_pContext, level.second));
		}	
	}

	return S_OK;
}

HRESULT CTool_Object::Show_PrototypeObjects()
{
	vector<string> objectPrototypes;
	m_pGameInstance->AddObjectPrototypesVector(&objectPrototypes);
	int selectedItem = -1; // 초기 선택 항목 없음

	for (int i = 0; i < objectPrototypes.size(); i++) {
		if (ImGui::Selectable(objectPrototypes[i].c_str(), selectedItem == i)) {
			selectedItem = i;
			// 오브젝트 생성 로직
			SpawnObjectAtZero(objectPrototypes[i]);
		}
	}

	return S_OK;
}

HRESULT CTool_Object::Show_LayerObjects()
{
	_uint currentLevel = m_pGameInstance->GetCurrentLevelIndex();
	ImGui::Columns(2, NULL, true); // 두 개의 컬럼 생성

	static int selectedItem = -1; // 초기 선택 항목 없음
	static vector<int> selectedObjects; // 선택된 객체들의 인덱스 벡터
	static vector<CGameObject*> selectedGameObjects; // 선택된 객체들의 포인터 벡터

	vector<pair<string, list<CGameObject*>>> objectLayersVector;
	m_pGameInstance->AddObjectLayersVector(currentLevel, &objectLayersVector);

	ImGui::BeginChild("LeftPane", ImVec2(150, 0), true); // 왼쪽 창 생성
	for (int i = 0; i < objectLayersVector.size(); i++) {
		if (ImGui::Selectable(objectLayersVector[i].first.c_str(), selectedItem == i)) {
			selectedItem = i;
			selectedObjects.clear(); // 새 레이어를 선택하면 오른쪽 창의 선택 초기화
			selectedGameObjects.clear(); // 선택된 객체 초기화
		}
	}

	ImGui::EndChild();
	ImGui::NextColumn(); // 다음 컬럼으로 이동

	ImGui::Checkbox("Picking_Create", &m_bPickingCheckBox);

	string layerName; // layerName 변수를 스코프 바깥으로 이동

	ImGui::BeginChild("RightPane", ImVec2(0, 0), true); // 오른쪽 창 생성
	if (selectedItem != -1) { // 유효한 레이어가 선택된 경우
		layerName = objectLayersVector[selectedItem].first;
		list<CGameObject*>& gameObjects = objectLayersVector[selectedItem].second;
		int index = 0;  // 게임 오브젝트의 번호를 표시하기 위한 인덱스
		for (auto it = gameObjects.begin(); it != gameObjects.end(); /* 빈 상태 */) {
			CGameObject* gameObject = *it;
			bool isSelected = std::find(selectedObjects.begin(), selectedObjects.end(), index) != selectedObjects.end();

			// 게임 오브젝트가 클릭되었는지 확인하고, 클릭된 경우 IMGUI의 선택 상태를 업데이트합니다.
			if (gameObject->GetIsPicking()) {
				// 기존 선택 상태를 초기화
				selectedObjects.clear();
				selectedGameObjects.clear();

				// 새로운 선택 상태를 설정
				selectedObjects.push_back(index);
				selectedGameObjects.push_back(gameObject);
				gameObject->SetPicking(false); // 클릭 상태 초기화
			}

			if (ImGui::Selectable((layerName + " " + std::to_string(index)).c_str(), isSelected)) {
				if (ImGui::GetIO().KeyCtrl) { // Ctrl 키가 눌려있는 상태에서만 다중 선택
					if (isSelected) {
						selectedObjects.erase(std::remove(selectedObjects.begin(), selectedObjects.end(), index), selectedObjects.end());
						selectedGameObjects.erase(std::remove(selectedGameObjects.begin(), selectedGameObjects.end(), gameObject), selectedGameObjects.end());
					}
					else {
						selectedObjects.push_back(index);
						selectedGameObjects.push_back(gameObject);
					}
				}
				else {
					selectedObjects.clear();
					selectedGameObjects.clear();
					selectedObjects.push_back(index);
					selectedGameObjects.push_back(gameObject);
				}
			}

			++it; // 삭제하지 않으면 다음 오브젝트로 이동
			index++;  // 다음 오브젝트에 대해 인덱스 증가
		}  
	}

	if (!selectedGameObjects.empty())
	{
		// 키 입력을 처리하여 포지션 이동
		_float3 deltaPosition = { 0.0f, 0.0f, 0.0f };
		if (m_bMovementEnabled) {
			_float offset = 0.1f;
			if (ImGui::IsKeyDown(ImGuiKey_W)) {
				deltaPosition.z += offset;
			}
			if (ImGui::IsKeyDown(ImGuiKey_S)) {
				deltaPosition.z -= offset;
			}
			if (ImGui::IsKeyDown(ImGuiKey_A)) {
				deltaPosition.x -= offset;
			}
			if (ImGui::IsKeyDown(ImGuiKey_D)) {
				deltaPosition.x += offset;
			}
			if (ImGui::IsKeyDown(ImGuiKey_Q)) {
				deltaPosition.y -= offset;
			}
			if (ImGui::IsKeyDown(ImGuiKey_E)) {
				deltaPosition.y += offset;
			}

			// 오브젝트 포지션 업데이트
			for (CGameObject* gameObject : selectedGameObjects) {
				CComponent* component = gameObject->Get_Component(TEXT("Com_Transform"));
				if (component != nullptr) {
					CTransform* transform = static_cast<CTransform*>(component);
					_vector currentPosition = transform->Get_State(CTransform::STATE_POSITION);

					_float3 fPos = {};
					XMStoreFloat3(&fPos, currentPosition);

					fPos.x += deltaPosition.x;
					fPos.y += deltaPosition.y;
					fPos.z += deltaPosition.z;

					_vector vSrc = XMLoadFloat3(&fPos);

					transform->Set_State(CTransform::STATE_POSITION, vSrc);
				}
			}
		}

		// 여기서 추가 동작을 수행할 수 있음 (예: 상세 정보 표시)
		_float3 averagePosition = { 0.0f, 0.0f, 0.0f };
		for (CGameObject* gameObject : selectedGameObjects) {
			CComponent* component = gameObject->Get_Component(TEXT("Com_Transform"));
			if (component != nullptr) {
				CTransform* transform = static_cast<CTransform*>(component);

				_vector vPos = transform->Get_State(CTransform::STATE_POSITION);
				_float3 vSrc = {};

				XMStoreFloat3(&vSrc, vPos);

				averagePosition.x += vSrc.x;
				averagePosition.y += vSrc.y;
				averagePosition.z += vSrc.z;
			}
		}

		averagePosition.x /= static_cast<float>(selectedGameObjects.size());
		averagePosition.y /= static_cast<float>(selectedGameObjects.size());
		averagePosition.z /= static_cast<float>(selectedGameObjects.size());

		// ImGui 스타일 및 너비 설정
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 4)); // 요소 간 간격 조정

		float sliderWidth = 150.0f; // 슬라이더의 너비
		float inputWidth = 50.0f;   // 입력 필드의 너비
		float sliderPower = 0.01f;  // 슬라이더 조정 민감도

		// 슬라이더 범위 설정
		float rangeX = 50.0f;
		float rangeY = 50.0f;
		float rangeZ = 50.0f;

		ImGui::Text("Position: ");
		ImGui::PushButtonRepeat(true); // 버튼 반복을 활성화

		// X 축 조정
		ImGui::PushItemWidth(sliderWidth);
		if (ImGui::Button("-X")) averagePosition.x -= 0.1f;
		ImGui::SameLine();
		ImGui::SliderFloat("##PosX", &averagePosition.x, 0, rangeX, "%.3f", sliderPower);
		ImGui::SameLine();
		if (ImGui::Button("+X")) averagePosition.x += 0.1f;
		ImGui::SameLine();
		ImGui::PushItemWidth(inputWidth);
		ImGui::InputFloat("##PosXInput", &averagePosition.x);
		ImGui::PopItemWidth();
		ImGui::PopItemWidth();

		// Y 축 조정
		ImGui::PushItemWidth(sliderWidth);
		if (ImGui::Button("-Y")) averagePosition.y -= 0.1f;
		ImGui::SameLine();
		ImGui::SliderFloat("##PosY", &averagePosition.y, 0, rangeY, "%.3f", sliderPower);
		ImGui::SameLine();
		if (ImGui::Button("+Y")) averagePosition.y += 0.1f;
		ImGui::SameLine();
		ImGui::PushItemWidth(inputWidth);
		ImGui::InputFloat("##PosYInput", &averagePosition.y);
		ImGui::PopItemWidth();
		ImGui::PopItemWidth();

		// Z 축 조정
		ImGui::PushItemWidth(sliderWidth);
		if (ImGui::Button("-Z")) averagePosition.z -= 0.1f;
		ImGui::SameLine();
		ImGui::SliderFloat("##PosZ", &averagePosition.z, 0, rangeZ, "%.3f", sliderPower);
		ImGui::SameLine();
		if (ImGui::Button("+Z")) averagePosition.z += 0.1f;
		ImGui::SameLine();
		ImGui::PushItemWidth(inputWidth);
		ImGui::InputFloat("##PosZInput", &averagePosition.z);
		ImGui::PopItemWidth();
		ImGui::PopItemWidth();

		ImGui::PopButtonRepeat(); // 버튼 반복을 비활성화

		for (CGameObject* gameObject : selectedGameObjects) {
			CComponent* component = gameObject->Get_Component(TEXT("Com_Transform"));
			if (component != nullptr) {
				CTransform* transform = static_cast<CTransform*>(component);
				transform->Set_State(CTransform::STATE_POSITION, XMVectorSet(averagePosition.x, averagePosition.y, averagePosition.z, 1.f));
			}
		}

		// 스케일 조절 슬라이더
		_float3 averageScale = { 0.0f, 0.0f, 0.0f };
		for (CGameObject* gameObject : selectedGameObjects) {
			CComponent* component = gameObject->Get_Component(TEXT("Com_Transform"));
			if (component != nullptr) {
				CTransform* transform = static_cast<CTransform*>(component);


				averageScale.x = transform->Get_Scaled().x;
				averageScale.y = transform->Get_Scaled().y;
				averageScale.z = transform->Get_Scaled().z;
			}
		}
		averageScale.x /= static_cast<float>(selectedGameObjects.size());
		averageScale.y /= static_cast<float>(selectedGameObjects.size());
		averageScale.z /= static_cast<float>(selectedGameObjects.size());

		ImGui::Text("Scale: ");
		ImGui::PushButtonRepeat(true); // 버튼 반복을 다시 활성화
		ImGui::PushItemWidth(sliderWidth);

		// 스케일 X 축 조정
		if (ImGui::Button("-SX")) averageScale.x -= 0.01f;
		ImGui::SameLine();
		ImGui::SliderFloat("##ScaleX", &averageScale.x, 0.1f, 10.0f, "%.3f", sliderPower);

		ImGui::SameLine();
		if (ImGui::Button("+SX")) averageScale.x += 0.01f;
		ImGui::SameLine();
		ImGui::PushItemWidth(inputWidth);
		if (ImGui::InputFloat("##ScaleXInput", &averageScale.x))

			ImGui::PopItemWidth();

		// 스케일 Y 축 조정
		if (ImGui::Button("-SY")) averageScale.y -= 0.01f;
		ImGui::SameLine();
		ImGui::SliderFloat("##ScaleY", &averageScale.y, 0.1f, 10.0f, "%.3f", sliderPower);
		ImGui::SameLine();
		if (ImGui::Button("+SY")) averageScale.y += 0.01f;
		ImGui::SameLine();
		ImGui::PushItemWidth(inputWidth);
		ImGui::InputFloat("##ScaleYInput", &averageScale.y);
		ImGui::PopItemWidth();

		// 스케일 Z 축 조정
		if (ImGui::Button("-SZ")) averageScale.z -= 0.01f;

		ImGui::SameLine();
		ImGui::SliderFloat("##ScaleZ", &averageScale.z, 0.1f, 10.0f, "%.3f", sliderPower);
		ImGui::SameLine();
		if (ImGui::Button("+SZ")) averageScale.z += 0.01f;
		ImGui::SameLine();
		ImGui::PushItemWidth(inputWidth);
		ImGui::InputFloat("##ScaleZInput", &averageScale.z);
		ImGui::PopItemWidth();

		ImGui::PopItemWidth();
		ImGui::PopButtonRepeat(); // 버튼 반복을 다시 비활성화
		ImGui::PopStyleVar(); // 스타일 변수 복원

		for (CGameObject* gameObject : selectedGameObjects) {
			CComponent* component = gameObject->Get_Component(TEXT("Com_Transform"));
			if (component != nullptr) {
				CTransform* transform = static_cast<CTransform*>(component);
				transform->Set_Scaled(averageScale.x, averageScale.y, averageScale.z); // 스케일 적용
			}
		}

		ImGui::Text("Rotation: ");
		ImGui::PushButtonRepeat(true); // 버튼 반복을 다시 활성화
		ImGui::PushItemWidth(sliderWidth);

		// 회전 X 축 조정
		if (ImGui::Button("-SX")) averageRotation.x -= 1.f;
		ImGui::SameLine();
		ImGui::SliderFloat("##RotationX", &averageRotation.x, 0.0f, 360.0f, "%.3f", sliderPower);
		ImGui::SameLine();
		if (ImGui::Button("+SX")) averageRotation.x += 1.f;
		ImGui::SameLine();
		ImGui::PushItemWidth(inputWidth);
		ImGui::InputFloat("##RotationXInput", &averageRotation.x);
		ImGui::PopItemWidth();

		// 회전 Y 축 조정
		if (ImGui::Button("-SY")) averageRotation.y -= 1.f;
		ImGui::SameLine();
		ImGui::SliderFloat("##RotationY", &averageRotation.y, 0.0f, 360.0f, "%.3f", sliderPower);
		ImGui::SameLine();
		if (ImGui::Button("+SY")) averageRotation.y += 1.f;
		ImGui::SameLine();
		ImGui::PushItemWidth(inputWidth);
		ImGui::InputFloat("##RotationYInput", &averageRotation.y);
		ImGui::PopItemWidth();

		// 회전 Z 축 조정
		if (ImGui::Button("-SZ")) averageRotation.z -= 1.f;

		ImGui::SameLine();
		ImGui::SliderFloat("##RotationZ", &averageRotation.z, 0.0f, 360.0f, "%.3f", sliderPower);
		ImGui::SameLine();
		if (ImGui::Button("+SZ")) averageRotation.z += 1.f;
		ImGui::SameLine();
		ImGui::PushItemWidth(inputWidth);
		ImGui::InputFloat("##RotationZInput", &averageRotation.z);
		ImGui::PopItemWidth();

		ImGui::PopButtonRepeat(); // 버튼 반복을 다시 비활성화

		for (CGameObject* gameObject : selectedGameObjects) {
			CComponent* component = gameObject->Get_Component(TEXT("Com_Transform"));
			if (component != nullptr) {

				CTransform* transform = static_cast<CTransform*>(component);

				if (averageRotation.x > 0.f)
					transform->Rotation({ 0.f, 1.f, 0.f ,0.f }, XMConvertToRadians(averageRotation.x));

				if (averageRotation.y > 0.f)
					transform->Rotation({ 1.f, 0.f, 0.f ,0.f }, XMConvertToRadians(averageRotation.y));

				if (averageRotation.z > 0.f)
					transform->Rotation({ 0.f, 0.f, 1.f ,0.f }, XMConvertToRadians(averageRotation.z));

			}
		}

		// 삭제 버튼 추가
		ImGui::Separator(); // 구분선 추가
		if (ImGui::Button("Delete Selected Object")) {
			Click_Button_Release(selectedGameObjects);
		}

		// 복사 버튼 추가
		ImGui::Separator(); // 구분선 추가
		if (ImGui::Button("Copy Selected Object")) {
			for (CGameObject* gameObject : selectedGameObjects) {
				// 레이어 이름과 함께 객체를 복사합니다.
				wstring_convert<std::codecvt_utf8<wchar_t>> converter;
				std::wstring wLayerName = converter.from_bytes(layerName);
				CopySelectedObject(wLayerName, gameObject);
			}
		}

		// 피킹 기능
		ImGui::Separator(); // 구분선 추가

		if (m_bPickingCheckBox)
		{
			if (m_pGameInstance->Key_Down(VK_LBUTTON))
			{
				for (CGameObject* gameObject : selectedGameObjects)
				{
					// 레이어 이름과 함께 객체를 복사합니다.
					wstring_convert<std::codecvt_utf8<wchar_t>> converter;
					std::wstring wLayerName = converter.from_bytes(layerName);
					
					m_vPickingPos = PickingPos();
					PickingSelectedObject(wLayerName, gameObject, m_vPickingPos);
				}
			}
		
		}
	}
		ImGui::EndChild();

		ImGui::Columns(1); // 열 병합

		for (auto& iter : objectLayersVector)
		{
			for (auto& iter2 : iter.second)
				Safe_Release(iter2);
		}
	
		return S_OK;
}

HRESULT CTool_Object::Click_Camera_FreeView(_bool bColliderToggle)
{
	CCamera_Free* pCamera = dynamic_cast<CCamera_Free*>(m_pGameInstance->Get_Layer(LEVELID::LEVEL_GAMEPLAY, TEXT("Layer_Camera")).back());
	pCamera->Set_FreeView(bColliderToggle);

	return S_OK;
}

HRESULT CTool_Object::SpawnObjectAtZero(const std::string& type)
{
	_uint currentLevel = m_pGameInstance->GetCurrentLevelIndex();

	// 문자열에서 "Prototype_GameObject_" 접두어 제거
	string prefix = "Prototype_GameObject_";
	string suffix;

	if (type.substr(0, prefix.size()) == prefix) {
		suffix = type.substr(prefix.size());
	}
	else {
		suffix = type; // 접두어가 없다면 전체 타입을 사용
	}

	// Layer 이름 생성
	string layerName = "Layer_" + suffix;

	wstring_convert<std::codecvt_utf8<wchar_t>> converter;
	wstring converted = converter.from_bytes(type);
	wstring wLayerName = converter.from_bytes(layerName);

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(currentLevel,  converted.c_str(),wLayerName)))
		return E_FAIL;

	return S_OK;
}

HRESULT CTool_Object::Click_Button_Release(std::vector<CGameObject*>& selectedGameObjects)
{
	for (auto it = selectedGameObjects.begin(); it != selectedGameObjects.end(); /* 빈 상태 */) {
		CGameObject* gameObject = *it;
		// 삭제 로직 호출
		gameObject->Set_Dead();
		
		//Safe_Release(gameObject); // 오브젝트 릴리즈
		it = selectedGameObjects.erase(it); // 리스트에서 오브젝트 제거
	}
	return S_OK;
}


HRESULT CTool_Object::CopySelectedObject(const std::wstring& layerName, CGameObject* sourceObject)
{
	// Get the current level index
	_uint currentLevel = m_pGameInstance->GetCurrentLevelIndex();

	// Extract the prototype suffix from the layer name
	std::wstring prototypeSuffix = layerName.substr(6); // "Layer_" 이후의 문자열 추출
	std::wstring prototypeTag = L"Prototype_GameObject_" + prototypeSuffix;

	// Copy the position and scale from the source object
	CComponent* sourceComponent = sourceObject->Get_Component(TEXT("Com_Transform"));
	CTransform* sourceTransform = static_cast<CTransform*>(sourceComponent);

	//sourceTransform->Get_State(CTransform::STATE_POSITION);

	FILEDATA copy_DESC{};
	copy_DESC.prototypeTag = prototypeTag;
	copy_DESC.layerName = layerName;
	copy_DESC.position = sourceTransform->Get_State(CTransform::STATE_POSITION);
	copy_DESC.scale = sourceTransform->Get_Scaled();
	copy_DESC.isParsing = false;

	// Create a new game object using the prototype
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(currentLevel,  prototypeTag.c_str() ,layerName.c_str(), &copy_DESC)))
		return E_FAIL;

	return S_OK;
}

HRESULT CTool_Object::PickingSelectedObject(const wstring& layerName, CGameObject* sourceObject ,_float3 vPos)
{
	// Get the current level index
	_uint currentLevel = m_pGameInstance->GetCurrentLevelIndex();

	// Extract the prototype suffix from the layer name
	std::wstring prototypeSuffix = layerName.substr(6); // "Layer_" 이후의 문자열 추출
	std::wstring prototypeTag = L"Prototype_GameObject_" + prototypeSuffix;

	// Copy the position and scale from the source object
	CComponent* sourceComponent = sourceObject->Get_Component(TEXT("Com_Transform"));
	CTransform* sourceTransform = static_cast<CTransform*>(sourceComponent);

	FILEDATA copy_DESC{};
	copy_DESC.prototypeTag = prototypeTag;
	copy_DESC.layerName = layerName;
	copy_DESC.scale = sourceTransform->Get_Scaled();
	copy_DESC.position = { vPos.x , vPos.y + sourceTransform->Get_Scaled().y / 2.f, vPos.z , 1.f};
	copy_DESC.isParsing = false;

	// Create a new game object using the prototype
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(currentLevel, prototypeTag.c_str(), layerName.c_str(), &copy_DESC)))
		return E_FAIL;

	return S_OK;
}

_float3 CTool_Object::PickingPos()
{
	_uint currentLevel = m_pGameInstance->GetCurrentLevelIndex();

	CTerrain* pTerrain = dynamic_cast<CTerrain*>(m_pGameInstance->Get_Layer(currentLevel, TEXT("Layer_BackGround")).front());
	m_vPickingPos = pTerrain->Get_TerrainPicking();
	

	CVIBuffer_Terrain* TerrainBuffer =  dynamic_cast<CVIBuffer_Terrain*>(m_pGameInstance->Get_Component(currentLevel, TEXT("Layer_BackGround"), TEXT("Com_VIBuffer")));

	_vector	vPos = {};
	TerrainBuffer->Picked(pTerrain->Get_Transform() , &vPos);

	_float3 vStorePos = {};
	XMStoreFloat3(&vStorePos, vPos);

	return vStorePos;
}

CTool_Object* CTool_Object::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CTool_Object* pInstance = new CTool_Object(pDevice , pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX(TEXT("Failed to Create : Tool_Object"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTool_Object::Free()
{
	__super::Free();
}
