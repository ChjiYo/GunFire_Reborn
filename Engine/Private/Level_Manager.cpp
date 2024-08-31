#include "..\Public\Level_Manager.h"
#include "Level.h"

#include "GameInstance.h"

CLevel_Manager::CLevel_Manager()	
	: m_pGameInstance{ CGameInstance::Get_Instance() }
{
	Safe_AddRef(m_pGameInstance);
}

HRESULT CLevel_Manager::Initialize()
{
	return S_OK;
}

void CLevel_Manager::Update(_float fTimeDelta)
{
	if (nullptr == m_pCurrentLevel)
		return;

	m_pCurrentLevel->Update(fTimeDelta);
}

HRESULT CLevel_Manager::Render()
{
	if (nullptr == m_pCurrentLevel)
		return E_FAIL;

	return m_pCurrentLevel->Render(); 
}

HRESULT CLevel_Manager::Change_Level(CLevel * pNewLevel)
{
	if(nullptr != m_pCurrentLevel)
		m_pGameInstance->Clear_LevelResources(m_pCurrentLevel->Get_LevelIndex());

  	Safe_Release(m_pCurrentLevel);


	m_pCurrentLevel = pNewLevel;

	return S_OK;
}

_uint CLevel_Manager::GetCurrentLevelIndex()
{
	return m_pCurrentLevel->Get_LevelIndex();
}

_uint CLevel_Manager::GetLoadingLevelIndex()
{
	return m_iloadingLevelIndex;
}

HRESULT CLevel_Manager::SetLoadingLevelIndex(_uint _level)
{
	m_iloadingLevelIndex = _level;
	return S_OK;
}

CLevel_Manager * CLevel_Manager::Create()
{
	CLevel_Manager*		pInstance = new CLevel_Manager();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX(TEXT("Failed to Created : CLevel_Manager"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


void CLevel_Manager::Free()
{
	Safe_Release(m_pGameInstance);
	Safe_Release(m_pCurrentLevel);

	__super::Free();
}

