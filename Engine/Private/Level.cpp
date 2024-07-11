#include "..\Public\Level.h"

#include "GameInstance.h"

CLevel::CLevel(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: m_pDevice{ pDevice }
	, m_pContext{ pContext }
	, m_pGameInstance{ CGameInstance::Get_Instance() }
{
	Safe_AddRef(m_pGameInstance);
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

HRESULT CLevel::Initialize()
{
	return S_OK;
}

void CLevel::Update(_float fTimeDelta)
{

}

HRESULT CLevel::Render()
{
	return S_OK;
}

HRESULT CLevel::Clear_Resources()
{
	return m_pGameInstance->Clear_LevelResources(m_iLevelIndex);	
}


HRESULT CLevel::ParseInitialize()
{
    // 파일 이름을 미리 배열로 정의
    const wchar_t* filePaths[] = {
        L"../Bin/LevelStaticObjects.txt",          // 0: LEVEL_STATIC
        L"../Bin/LevelLoadingObjects.txt",         // 1: LEVEL_LOADING
        L"../Bin/LevelLogoObjects.txt",             // 2 : LEVEL_LOGO
        L"../Bin/LevelGamePlayObjects.txt",        // 3: LEVEL_GAMEPLAY
        L"../Bin/DefaultLevelObjects.txt"          // 8: LEVEL_END (또는 기타)
    };

     // 배열 인덱스를 사용하여 파일 경로 가져오기
    wstring filePath;
    if (m_iLevelIndex >= 0 && m_iLevelIndex < _countof(filePaths)) {
        filePath = filePaths[m_iLevelIndex];
    }
    else {
        filePath = L"../Bin/DefaultLevelObjects.txt"; // 기본값
    }


    vector<FILEDATA>* pvecFileData = static_cast<vector<FILEDATA>*>(m_pGameInstance->LoadObjects(filePath.c_str()));
    if (pvecFileData == nullptr)
    {
        return E_FAIL;
    }

    size_t totalSize = pvecFileData->size() * sizeof(FILEDATA);
    for (auto& iter : *pvecFileData)
    {
         if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(iter.levelIndex, iter.prototypeTag, iter.layerName, &iter)))
            return E_FAIL;
    }

    return S_OK;
}

void CLevel::Free()
{
	__super::Free();

	Safe_Release(m_pGameInstance);
	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
}
