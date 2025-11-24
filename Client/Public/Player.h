#pragma once

#include "Client_Defines.h"
#include "ContainerObject.h"
#include "Light.h"

#include <deque>

BEGIN(Engine)
class CShader;
class CModel;
class CBinModel;
class CKeyState;
class CCollider;
class CNavigation;
END

BEGIN(Client)

class CPlayer final : public CContainerObject
{
public:
	enum PARTID {  PART_WEAPON, PART_EFFECT, PART_END };
	enum WEAPONID { PISTOL , SNIPER ,SHURIKEN ,BOOMGUN, WEAPON_END };
	enum SLOTID {ONE , TWO , THREE , SLOT_END};
public:
	typedef struct :public CContainerObject::CONTAINEROBJECT_DESC
	{
		_int iHp = { 0 };
		_int iShield = { 0 };

		_int iMaxHp = { 0 };
		_int iMaxShield = { 0 };

	}PLAYER_DESC;

private:
	CPlayer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CPlayer(const CPlayer& Prototype);
	virtual ~CPlayer() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	PLAYER_DESC Get_Player_Desc() { return Player_Desc; }
	WEAPONID Get_WeaponID(_uint iSlotIndex) {
		int a = 0;
		return m_eWeaponIDArray[iSlotIndex]; 
	}

	void		Set_Damage(_int iDamage)
	{
		Player_Desc.iShield <= 0 ? Player_Desc.iHp += iDamage, Player_Desc.iShield = 0 : Player_Desc.iShield += iDamage;

		m_fShieldRecoveryTimer = 0.f;

		m_bHit = true;
	}

	WEAPONID	Get_WeaponID() { 

		return m_eWeaponID; 
	}


	void ChangeWeapon(WEAPONID eWeaponID)
	{
		m_bWeaponChange = true;

		for (int i = 0; i < 2; ++i)
		{
			if (m_eWeaponIDArray[i] == WEAPON_END)
			{
				m_eWeaponIDArray[i] = eWeaponID;
				m_eWeaponID = m_eWeaponIDArray[i];
				return;
			}
		}

		if (m_eSlotID == THREE)
		{
			m_eWeaponIDArray[ONE] = eWeaponID;
			m_eWeaponID = m_eWeaponIDArray[ONE];

			//m_pDataMgr->Set_WeaponID(ONE, eWeaponID);
		}
		else
		{
			m_eWeaponIDArray[m_eSlotID] = eWeaponID;
			m_eWeaponID = m_eWeaponIDArray[m_eSlotID];

			//m_pDataMgr->Set_WeaponID(m_eSlotID, eWeaponID);
		}
	}

public:
	void WeaponUpgread(CPlayer::SLOTID eID)
	{
		m_vUpgreadCnt[eID] += 1; 
		m_vCurrentDamage[m_eWeaponIDArray[eID]] = m_vDamage[m_eWeaponIDArray[eID]] + (m_vUpgreadCnt[eID] * 10);
		int a = 10;
	}

	_uint UpgreadCnt(CPlayer::WEAPONID eID) { 
		int a = 10;
		return m_vUpgreadCnt[eID]; 
	}
	_uint WeaponDamage(CPlayer::WEAPONID eID) { return m_vDamage[eID]; }
	_uint CurrentDamage(CPlayer::WEAPONID eID) {
		return m_vCurrentDamage[eID]; 
	}

	_float Get_HitTimer() { return m_fHitTimer; }
	_bool Get_Hit() { return m_bHit; }

public:
	SLOTID Get_Slot() { return m_eSlotID; }

public:
	_bool Get_Dash() { return m_bDash; }
	_float Get_DashTimer() { return m_fDashTimer; }

private:
	void ShieldRecovery();

private:
	CNavigation*		m_pNavigationCom = { nullptr };
	CCollider*			m_pColliderCom = { nullptr };

	class CData_Manager*		m_pDataMgr = { nullptr };

private:
	PLAYER_DESC			Player_Desc = {};
	_bool				m_bJump = { false };
	_bool				m_bWeaponChange = { true };

	WEAPONID			m_eWeaponID = { WEAPON_END };
	WEAPONID			m_ePrevWeaponID = { WEAPON_END };

	_float				m_fShieldRecoveryTimer = { 0.f };
	_float				m_fInvincibleTime = { 0.f };
	
	WEAPONID			m_eWeaponIDArray[3] = { SHURIKEN ,SNIPER ,PISTOL };

	SLOTID				m_eSlotID = { SLOT_END };

	_bool				m_bDash = { false };
	_float				m_fDashTimer = { 0.f };

	deque<_matrix> m_StackPos = {};

	_float m_fPosSaveTimer = { 0.f };
	_float m_fMoveSkillTimer = { 0.f };
	_bool	m_bBackPos = {false};

	CLight* m_pLight = {};
	_uint m_vDamage[WEAPON_END] = {100 , 200 , 50 , 25};
	_uint m_vCurrentDamage[WEAPON_END] = { 100 , 200 , 50 , 25 };
	_uint m_vUpgreadCnt[WEAPON_END] = { 0 , 0 , 0 , 0 };
	
	vector<CPartObject*> vecParts = {};

	_bool m_bHit = { false };
	_float m_fHitTimer = { 0.f };
	_uint m_HitCnt = { 0 };

private:
	HRESULT Ready_Components();

	void Key_State(_float fTimeDelta);

	void Player_Move(_float fTimeDelta);
	void Player_Jump(_float fTimeDelta);
	void Player_Weapon_Change();
	void Dash(_float fTimeDelta);

private:
	void InitPostion(_fvector vPos, _fvector vLook)
	{
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);
		m_pTransformCom->LookAt(vLook);
	}

public:
	static CPlayer* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END

