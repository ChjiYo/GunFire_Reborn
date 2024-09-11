#include "stdafx.h"

#include "..\Public\NPC.h"
#include "GameInstance.h"

CNPC::CNPC(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject{ pDevice, pContext }
{

}

CNPC::CNPC(const CNPC& Prototype)
	: CGameObject{ Prototype }
{

}

HRESULT CNPC::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CNPC::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CNPC::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);

}

void CNPC::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);

}

void CNPC::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);


}

HRESULT CNPC::Render()
{
	return S_OK;
}

HRESULT CNPC::Ready_Components()
{



	return S_OK;
}


void CNPC::Free()
{
	__super::Free();
}
