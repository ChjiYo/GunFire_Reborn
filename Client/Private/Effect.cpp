#include "stdafx.h"

#include "Effect.h"
#include "Navigation.h"

CEffect::CEffect(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CPartObject{ pDevice ,pContext }
{
}

CEffect::CEffect(const CEffect& Prototype)
	:CPartObject{ Prototype }
{
}

HRESULT CEffect::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CEffect::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CEffect::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

void CEffect::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);
}

void CEffect::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CEffect::Render()
{
	return S_OK;
}

HRESULT CEffect::Ready_Components()
{
	return S_OK;
}

void CEffect::Free()
{
	__super::Free();
}
