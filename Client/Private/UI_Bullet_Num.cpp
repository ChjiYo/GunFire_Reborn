#include "stdafx.h"

#include "UI_Bullet_Num.h"
#include "Player.h"
#include "Weapon_Player.h"

#include "GameInstance.h"


CUI_Bullet_Num::CUI_Bullet_Num(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CUIObject{ pDevice ,pContext }
{
}

CUI_Bullet_Num::CUI_Bullet_Num(const CUI_Bullet_Num& Prototype)
	:CUIObject{ Prototype }
{
}

HRESULT CUI_Bullet_Num::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Bullet_Num::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	UI_DESC* pDesc = static_cast<UI_DESC*>(pArg);
	m_iNum = pDesc->iNumUI;


	m_fSizeX = 25.f;
	m_fSizeY = 25.f;
	m_fPosX = 1450.f + m_iNum * 25.f;
	m_fPosY = 680.f;

	__super::Set_UI_Setting(m_fSizeX, m_fSizeY, m_fPosX, m_fPosY);

	return S_OK;
}

void CUI_Bullet_Num::Priority_Update(_float fTimeDelta)
{

}

void CUI_Bullet_Num::Update(_float fTimeDelta)
{
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(m_pGameInstance->Get_Layer(m_pGameInstance->GetLoadingLevelIndex(), TEXT("Layer_Player")).back());
	_uint iCurrentBullet = dynamic_cast<CWeapon_Player*>(pPlayer->Get_Parts().at(CPlayer::PART_WEAPON))->Get_CurrentBullet();
	_uint iMaxBullet = dynamic_cast<CWeapon_Player*>(pPlayer->Get_Parts().at(CPlayer::PART_WEAPON))->Get_MaxBullet();

	switch (m_iNum)
	{
	case 0:
		m_iTextureIndex = iCurrentBullet /= 10;
		break;

	case 1:
		m_iTextureIndex = iCurrentBullet %= 10;
		break;

	case 2:
		m_iTextureIndex = 11;
		break;

	case 3:

		if (pPlayer->Get_WeaponID() == CPlayer::PISTOL)
		{
			m_iTextureIndex = 10;
			m_fSizeX = 50.f;
			m_fSizeY = 25.f;
			m_fPosX = 1475.f + m_iNum * 25.f;
			__super::Set_UI_Setting(m_fSizeX, m_fSizeY, m_fPosX, m_fPosY);
		}
		else
		{
		
			m_fSizeX = 25.f;
			m_fSizeY = 25.f;
			m_fPosX = 1450.f + m_iNum * 25.f;
			m_iTextureIndex = iMaxBullet /= 10;

			__super::Set_UI_Setting(m_fSizeX, m_fSizeY, m_fPosX, m_fPosY);
		}
		break;

	case 4:
		if (pPlayer->Get_WeaponID() == CPlayer::PISTOL)
		{
			m_fPosX = -100.f;
			m_fPosY = -100.f;
			__super::Set_UI_Setting(m_fSizeX, m_fSizeY, m_fPosX, m_fPosY);
		}
		else
		{
			m_fPosX = 1450.f + m_iNum * 25.f;
			m_fPosY = 680.f;

			__super::Set_UI_Setting(m_fSizeX, m_fSizeY, m_fPosX, m_fPosY);
		}
			m_iTextureIndex = iMaxBullet %= 10;
		break;

	}
}

void CUI_Bullet_Num::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderObject(CRenderer::RG_UI, this);
}

HRESULT CUI_Bullet_Num::Render()
{
	if (FAILED(__super::Bind_ShaderResources()))
		return E_FAIL;;

	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", m_iTextureIndex)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Begin(0)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Bind_Buffers()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_Bullet_Num::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	/* For.Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Number"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;
}

CUI_Bullet_Num* CUI_Bullet_Num::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_Bullet_Num* pInstatnce = new CUI_Bullet_Num(pDevice, pContext);

	if (FAILED(pInstatnce->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CUI_Bullet_Num"));
		Safe_Release(pInstatnce);
	}

	return pInstatnce;
}

CGameObject* CUI_Bullet_Num::Clone(void* pArg)
{
	CUI_Bullet_Num* pInstatnce = new CUI_Bullet_Num(*this);

	if (FAILED(pInstatnce->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloend : CUI_Bullet_Num"));
		Safe_Release(pInstatnce);
	}

	return pInstatnce;
}

void CUI_Bullet_Num::Free()
{
	__super::Free();
}
