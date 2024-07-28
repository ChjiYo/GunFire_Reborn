#include "stdafx.h"
#include "..\Public\Camera_Free.h"

#include "GameInstance.h"

CCamera_Free::CCamera_Free(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CCamera { pDevice, pContext }
{

}

CCamera_Free::CCamera_Free(const CCamera_Free & Prototype)
	: CCamera { Prototype }
{

}

HRESULT CCamera_Free::Initialize_Prototype()
{
	if(FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CCamera_Free::Initialize(void * pArg)
{
	m_fMouseSensor = static_cast<CCamera_Free::CAMERA_FREE_DESC*>(pArg)->fSensor;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	CAMERA_FREE_DESC* Desc = static_cast<CAMERA_FREE_DESC*>(pArg);
	m_pPlayerTransform = Desc->pPlayerTransform;
	Safe_Release(m_pPlayerTransform);

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, { 5.f , 5.5f , 0.f, 1.f });

	return S_OK;
}

void CCamera_Free::Priority_Update(_float fTimeDelta)
{
	if (m_bCameraMode)
	{
		m_pTransformCom->Set_Speed(5.f);
		Position();
	}
	else
	{
		m_pTransformCom->Set_Speed(10.f);
		FreeView_Camera(fTimeDelta);
	}

	__super::Priority_Update(fTimeDelta);
}

void CCamera_Free::Update(_float fTimeDelta)
{
	
}

void CCamera_Free::Late_Update(_float fTimeDelta)
{
	
}

HRESULT CCamera_Free::Render()
{
	return S_OK;
}

void CCamera_Free::FreeView_Camera(_float fTimeDelta)
{
	//마우스
	if (m_pGameInstance->Get_DIKeyState(DIK_LSHIFT) & 0x80)
	{
		_long		MouseMove = {};

		if (MouseMove = m_pGameInstance->Get_DIMouseMove(DIMM_X))
		{
			m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), m_fMouseSensor * MouseMove * fTimeDelta);
		}

		if (MouseMove = m_pGameInstance->Get_DIMouseMove(DIMM_Y))
		{
			m_pTransformCom->Turn(m_pTransformCom->Get_State(CTransform::STATE_RIGHT), m_fMouseSensor * MouseMove * fTimeDelta);
		}
	}

	//키보드
	if (m_pGameInstance->Get_DIKeyState(DIK_A) & 0x80)
	{
		m_pTransformCom->Go_Left(fTimeDelta);
	}

	if (m_pGameInstance->Get_DIKeyState(DIK_D) & 0x80)
	{
		m_pTransformCom->Go_Right(fTimeDelta);
	}

	if (m_pGameInstance->Get_DIKeyState(DIK_W) & 0x80)
	{
		m_pTransformCom->Go_Straight(fTimeDelta);
	}

	if (m_pGameInstance->Get_DIKeyState(DIK_S) & 0x80)
	{
		m_pTransformCom->Go_Backward(fTimeDelta);
	}
}

void CCamera_Free::Position()
{
	_vector vRight = m_pPlayerTransform->Get_State(CTransform::STATE_RIGHT);
	_vector vUp = m_pPlayerTransform->Get_State(CTransform::STATE_UP);
	_vector vLook = m_pPlayerTransform->Get_State(CTransform::STATE_LOOK);
	_vector vPlayerPosition = m_pPlayerTransform->Get_State(CTransform::STATE_POSITION);

	_vector vWeaponRight = vRight * -0.3f;
	_vector vWeaponUp = vUp * 0.45f;
	_vector vWeaponLook = vLook * -0.5f;
	_vector vWeaponPosition = vPlayerPosition + vWeaponRight + vWeaponUp + vWeaponLook;

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vWeaponPosition);

	m_pTransformCom->LookAt(vPlayerPosition + vLook * 15.f);
}

CCamera_Free * CCamera_Free::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CCamera_Free*		pInstance = new CCamera_Free(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CCamera_Free"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CCamera_Free::Clone(void * pArg)
{
	CCamera_Free*		pInstance = new CCamera_Free(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CCamera_Free"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCamera_Free::Free()
{
	Safe_Release(m_pPlayerTransform);

	__super::Free();
}
