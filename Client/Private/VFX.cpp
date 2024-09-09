#include "stdafx.h"

#include "VFX.h"
#include "Navigation.h"

CVFX::CVFX(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CGameObject{ pDevice ,pContext }
{
}

CVFX::CVFX(const CVFX& Prototype)
	:CGameObject{ Prototype }
{
}

HRESULT CVFX::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CVFX::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CVFX::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

void CVFX::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);
}

void CVFX::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CVFX::Render()
{
	return S_OK;
}

HRESULT CVFX::Ready_Components()
{
	return S_OK;
}

void CVFX::Free()
{
	__super::Free();
}
