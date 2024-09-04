#include "stdafx.h"

#include "Data_Manager.h"

IMPLEMENT_SINGLETON(CData_Manager)

CData_Manager::CData_Manager()
{
}

HRESULT CData_Manager::Initialize()
{


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
	CData_Manager::Get_Instance()->Destroy_Instance();

	__super::Free();
}
