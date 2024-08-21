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

	if (m_bIsPasingObject)
	{
		FILEDATA* fileData = static_cast<FILEDATA*>(pArg);
		m_pTransformCom->Set_Scaled(fileData->scale.x, fileData->scale.y, fileData->scale.z);
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSetW(fileData->position, 1.f));
		m_pTransformCom->Rotation({ 0.f, 1.f, 0.f }, fileData->fRotation);
	}

	if (pArg != nullptr)
	{
		// pArg가 FILEDATA 타입인지 확인
		FILEDATA* pFileData = reinterpret_cast<FILEDATA*>(pArg);
		if (pFileData != nullptr && !pFileData->isParsing) // isParsing 값을 확인
		{
			// 전달된 위치와 스케일 적용
			CComponent* component = Get_Component(TEXT("Com_Transform"));
			CTransform* transform = static_cast<CTransform*>(component);

			transform->Set_State(CTransform::STATE_POSITION, pFileData->position);
			transform->Set_Scaled(pFileData->scale.x, pFileData->scale.y, pFileData->scale.z);
			m_pTransformCom->Rotation({ 0.f, 1.f, 0.f }, pFileData->fRotation);
			return S_OK;
		}
	}

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
