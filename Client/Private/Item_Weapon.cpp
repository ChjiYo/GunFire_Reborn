#include "stdafx.h"

#include "Item_Weapon.h"
#include "GameInstance.h"

#include "Player.h"

CItem_Weapon::CItem_Weapon(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CItem{ pDevice , pContext }
{
}

CItem_Weapon::CItem_Weapon(const CItem_Weapon& pPrototype)
	:CItem{ pPrototype }
{
}

HRESULT CItem_Weapon::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CItem_Weapon::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CItem_Weapon::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

void CItem_Weapon::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);

	CTransform* pPlayerTranfsorm = dynamic_cast<CTransform*>(m_pGameInstance->Get_Component(LEVEL_STATIC, TEXT("Layer_Player"), TEXT("Com_Transform")));
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(m_pGameInstance->Get_Layer(LEVEL_STATIC, TEXT("Layer_Player")).back());

	_float fDistance = XMVectorGetX(XMVector3Length(m_pTransformCom->Get_State(CTransform::STATE_POSITION) - pPlayerTranfsorm->Get_State(CTransform::STATE_POSITION)));

	if (fabs(fDistance) <= 1.f && m_pGameInstance->Key_Down('F'))
	{
		pPlayer->ChangeWeapon(static_cast<CPlayer::WEAPONID>(m_eWeaponID));
		m_bDead = true;
	}
}

void CItem_Weapon::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CItem_Weapon::Render()
{
	return S_OK;
}


void CItem_Weapon::Free()
{
	__super::Free();
}
