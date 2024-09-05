#include "stdafx.h"

#include "Data_Manager.h"
#include "GameInstance.h"

IMPLEMENT_SINGLETON(CData_Manager)

CData_Manager::CData_Manager()
	: m_pGameInstance{ CGameInstance::Get_Instance() }
{
	Safe_AddRef(m_pGameInstance);
}

HRESULT CData_Manager::Initialize()
{
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(m_pGameInstance->Get_Layer(m_pGameInstance->GetCurrentLevelIndex(), TEXT("Layer_Player")).back());
	m_ePlayerDesc =  pPlayer->Get_Player_Desc();

	return S_OK;
}

CData_Manager* CData_Manager::Create()
{
	CData_Manager* pInstance = new CData_Manager();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX(TEXT("Failed to Created : CData_Manager"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CData_Manager::Free()
{
	//CData_Manager::Get_Instance()->Destroy_Instance();
	Safe_Release(m_pGameInstance);

	__super::Free();
}
