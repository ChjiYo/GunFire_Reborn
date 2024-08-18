#include "stdafx.h"

#include "Item_Weapon.h"

CItem_Weapon::CItem_Weapon(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CItem{ pDevice , pContext }
{
}

CItem_Weapon::CItem_Weapon(const CItem_Weapon& pPrototype)
	:CItem{ pPrototype }
{
}

HRESULT CItem_Weapon::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CItem_Weapon::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CItem_Weapon::Priority_Update(_float fTimeDelta)
{
}

void CItem_Weapon::Update(_float fTimeDelta)
{
}

void CItem_Weapon::Late_Update(_float fTimeDelta)
{
}

HRESULT CItem_Weapon::Render()
{
	return S_OK;
}


void CItem_Weapon::Free()
{
	__super::Free();
}
