#include "stdafx.h"
#include "..\Public\Monster.h"

#include "GameInstance.h"

CMonster::CMonster(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CContainerObject{ pDevice, pContext }
{

}

CMonster::CMonster(const CMonster & Prototype)
	: CContainerObject{ Prototype }
{

}

HRESULT CMonster::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CMonster::Initialize(void * pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CMonster::Priority_Update(_float fTimeDelta)
{
		
}

void CMonster::Update(_float fTimeDelta)
{

}

void CMonster::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderObject(CRenderer::RG_NONBLEND, this);
}

HRESULT CMonster::Render()
{
	return S_OK;
}

void CMonster::Free()
{
	__super::Free();
}
