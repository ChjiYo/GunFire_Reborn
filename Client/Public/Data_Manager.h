#pragma once

#include "Client_Defines.h"
#include "Base.h"

#include "Player.h"
#include "Weapon_Player.h"

BEGIN(Client)

class CData_Manager final : public CBase
{
public:
	enum WEAPONID { PISTOL, SNIPER, SHURIKEN, BOOMGUN, WEAPON_END };

	DECLARE_SINGLETON(CData_Manager)
private:
	CData_Manager();
	virtual ~CData_Manager() = default;

public:
	HRESULT Initialize();


private:
	CPlayer::PLAYER_DESC	m_ePlayerDesc = {};
	CWeapon_Player::PLAYER_WEAPON_DESC m_eWeaponDesc[WEAPON_END] = {};

public:
	static CData_Manager* Create();
	virtual void Free() override;
};

END