#include "stdafx.h"

#include "Wall_State1.h"

CWall_State1::CWall_State1(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CElement { pDevice , pContext }
{
}

CWall_State1::CWall_State1(const CWall_State1& pPrototype)
	:CElement{ pPrototype }
{
}

HRESULT CWall_State1::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CWall_State1::Initialize(void* pArg)
{
	return S_OK;
}

void CWall_State1::Priority_Update(_float fTimeDelta)
{
}

void CWall_State1::Update(_float fTimeDelta)
{
}

void CWall_State1::Late_Update(_float fTimeDelta)
{
}

HRESULT CWall_State1::Render()
{
	return S_OK;
}

CWall_State1* CWall_State1::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CWall_State1* pInstance = new CWall_State1(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CWall_State1"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CWall_State1::Clone(void* pArg)
{
	CWall_State1* pInstance = new CWall_State1(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CWall_State1"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CWall_State1::Free()
{
	__super::Free();
}
