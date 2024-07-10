#include "stdafx.h"

#include "Wall_Stage1.h"

CWall_Stage1::CWall_Stage1(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CElement { pDevice , pContext }
{
}

CWall_Stage1::CWall_Stage1(const CWall_Stage1& pPrototype)
	:CElement{ pPrototype }
{
}

HRESULT CWall_Stage1::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CWall_Stage1::Initialize(void* pArg)
{
	return S_OK;
}

void CWall_Stage1::Priority_Update(_float fTimeDelta)
{
}

void CWall_Stage1::Update(_float fTimeDelta)
{
}

void CWall_Stage1::Late_Update(_float fTimeDelta)
{
}

HRESULT CWall_Stage1::Render()
{
	return S_OK;
}

CWall_Stage1* CWall_Stage1::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CWall_Stage1* pInstance = new CWall_Stage1(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CWall_State1"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CWall_Stage1::Clone(void* pArg)
{
	CWall_Stage1* pInstance = new CWall_Stage1(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CWall_State1"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CWall_Stage1::Free()
{
	__super::Free();
}
