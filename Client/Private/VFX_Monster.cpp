#include "stdafx.h"

#include "VFX_Monster.h"
#include "Navigation.h"

CVFX_Monster::CVFX_Monster(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CEffect{ pDevice ,pContext }
{
}

CVFX_Monster::CVFX_Monster(const CVFX_Monster& Prototype)
	:CEffect{ Prototype }
{
}

HRESULT CVFX_Monster::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CVFX_Monster::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CVFX_Monster::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

void CVFX_Monster::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);
}

void CVFX_Monster::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CVFX_Monster::Render()
{
	return S_OK;
}

HRESULT CVFX_Monster::Ready_Components()
{
	return S_OK;
}

void CVFX_Monster::Free()
{
	__super::Free();
}
