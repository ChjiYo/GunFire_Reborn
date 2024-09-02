#include "stdafx.h"

#include "Fire_Light.h"

#include "GameInstance.h"

CFire_Light::CFire_Light(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CElement{ pDevice , pContext }
{
}

CFire_Light::CFire_Light(const CFire_Light& pPrototype)
	:CElement{ pPrototype }
{
}

HRESULT CFire_Light::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CFire_Light::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	_float4 vTmp = {};
	XMStoreFloat4(&vTmp, m_pTransformCom->Get_State(CTransform::STATE_POSITION));
	LIGHT_DESC			LightDesc{};

	ZeroMemory(&LightDesc, sizeof(LIGHT_DESC));
	LightDesc.eType = LIGHT_DESC::TYPE_POINT;
	LightDesc.vPosition = vTmp;
	LightDesc.fRange = 15.f;
	LightDesc.vDiffuse = _float4(1.0f, 0.0f, 0.0f, 0.75f);
	LightDesc.vAmbient = _float4(0.25f, 0.25f, 0.f, 1.f);
	LightDesc.vSpecular = _float4(0.f, 0.f, 0.f, 1.f);

	if (FAILED(m_pGameInstance->Add_Light(LightDesc)))
		return E_FAIL;

	return S_OK;
}

void CFire_Light::Priority_Update(_float fTimeDelta)
{
}

void CFire_Light::Update(_float fTimeDelta)
{
}

void CFire_Light::Late_Update(_float fTimeDelta)
{
}

HRESULT CFire_Light::Render()
{
	return S_OK;
}

CFire_Light* CFire_Light::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CFire_Light* pInstance = new CFire_Light(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CFire_Light"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CFire_Light::Clone(void* pArg)
{
	CFire_Light* pInstance = new CFire_Light(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CFire_Light"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CFire_Light::Free()
{
	__super::Free();
}
