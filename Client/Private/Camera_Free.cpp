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

	if (FAILED(Ready_Components()))
		return E_FAIL;

	CAMERA_FREE_DESC* Desc = static_cast<CAMERA_FREE_DESC*>(pArg);
	m_pPlayerTransform = Desc->pPlayerTransform;
	Safe_AddRef(m_pPlayerTransform);

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, { 5.f , 5.5f , 0.f, 1.f });
	m_fScopeLevel = 10.35f;

	return S_OK;
}

void CCamera_Free::Priority_Update(_float fTimeDelta)
{
	Camera_State(fTimeDelta);

	//__super::Priority_Update(fTimeDelta);
	m_pGameInstance->Set_Transform(CPipeLine::D3DTS_VIEW, m_pTransformCom->Get_WorldMatrix_Inverse());
	m_pGameInstance->Set_Transform(CPipeLine::D3DTS_PROJ, XMMatrixPerspectiveFovLH(m_fFovy, m_fViewportWidth / m_fViewportHeight, m_fNear, m_fFar));

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

HRESULT CCamera_Free::Ready_Components()
{
	return S_OK;
}

void CCamera_Free::Camera_State(_float fTimeDelta , _float fMul)
{
	//Position({ -0.3f , 0.35f, -0.35f});
	switch (m_eCameraType)
	{
	case PLAYER:
		m_fFovy = XMConvertToRadians(60.0f);
		Position({ -0.3f , 0.35f, -0.35f });
		break;

	case FREE:
		FreeView_Camera(fTimeDelta);
		break;

	case SCOPE:
		Scope_Camera(fTimeDelta, fMul);
		break;
	}
}

void CCamera_Free::FreeView_Camera(_float fTimeDelta)
{
	m_pTransformCom->Set_Speed(10.f);

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

void CCamera_Free::Position(_float3 vOffSet)
{
	m_pTransformCom->Set_Speed(5.f);

	_vector vRight = m_pPlayerTransform->Get_State(CTransform::STATE_RIGHT);
	_vector vUp = m_pPlayerTransform->Get_State(CTransform::STATE_UP);
	_vector vLook = m_pPlayerTransform->Get_State(CTransform::STATE_LOOK);
	_vector vPlayerPosition = m_pPlayerTransform->Get_State(CTransform::STATE_POSITION);

	_vector vWeaponRight = vRight * vOffSet.x;
	_vector vWeaponUp = vUp * vOffSet.y;
	_vector vWeaponLook = vLook * vOffSet.z;
	_vector vWeaponPosition = vPlayerPosition + vWeaponRight + vWeaponUp + vWeaponLook;

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, vWeaponPosition);

	m_pTransformCom->LookAt(vPlayerPosition + vLook * 300.f);
}

void CCamera_Free::Scope_Camera(_float fTimeDelta, _float fMul)
{ 
	m_fFovy = XMConvertToRadians(30.0f);

	Position({ -0.3f , 0.35f,  1.35f });
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
