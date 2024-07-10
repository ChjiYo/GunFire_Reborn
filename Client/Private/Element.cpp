#include "stdafx.h"

#include "Element.h"

CElement::CElement(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CGameObject{ pDevice , pContext }
{
}

CElement::CElement(const CElement& pPrototype)
	:CGameObject{ pPrototype }
{
}

HRESULT CElement::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CElement::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CElement::Priority_Update(_float fTimeDelta)
{
}

void CElement::Update(_float fTimeDelta)
{
}

void CElement::Late_Update(_float fTimeDelta)
{
}

HRESULT CElement::Render()
{
	return S_OK;
}


void CElement::Free()
{
	__super::Free();
}
