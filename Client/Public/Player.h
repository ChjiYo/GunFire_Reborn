#pragma once

#include "Client_Defines.h"
#include "ContainerObject.h"

BEGIN(Engine)
class CShader;
class CModel;
class CBinModel;
class CKeyState;
class CCollider;
class CState_Machine;
class CNavigation;
END

BEGIN(Client)

class CPlayer final : public CContainerObject
{
public:
	enum PARTID {  PART_WEAPON, PART_EFFECT, PART_END };
	enum WEAPONID { PISTOL , SNIPER ,SHURIKEN ,BOOMGUN, WEAPON_END };
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
	void		Set_Damage(_uint iDamage)
	{
		if(Player_Desc.iShield <= 0 )
			Player_Desc.iHp += iDamage;
		else 
			Player_Desc.iShield += iDamage;	

		m_fShieldRecoveryTimer = 0.f;
	}

	WEAPONID	Get_WeaponID() { return m_eWeaponID; }

private:
	void ShieldRecovery();


private:
	CState_Machine*		m_pState = { nullptr };
	CNavigation*		m_pNavigationCom = { nullptr };
	CCollider*			m_pColliderCom = { nullptr };

private:
	PLAYER_DESC			Player_Desc = {};
	_bool				m_bJump = { false };
	_bool				m_bWeaponChange = { true };

	_float				m_fShieldRecoveryTimer = { 0.f };
	WEAPONID			m_eWeaponID = { WEAPON_END };
	

private:
	HRESULT Ready_Components();
	HRESULT Ready_State();

	void Key_State(_float fTimeDelta);

	void Player_Move(_float fTimeDelta);
	void Player_Jump(_float fTimeDelta);
	void Player_Weapon_Change();

public:
	static CPlayer* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END

