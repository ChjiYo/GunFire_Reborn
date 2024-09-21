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


public:
	CPlayer::PLAYER_DESC Get_PlayerDesc(){ return m_ePlayerDesc; }
	void SaveBullet(CPlayer::WEAPONID eID, _uint iMax, _uint iCurr) { m_iMaxBullet[eID] = iMax; m_iCurrentBullet[eID] = iCurr; }

	_uint Get_MaxBullet(CPlayer::WEAPONID eID) { return m_iMaxBullet[eID]; }
	_uint Get_CurrBullet(CPlayer::WEAPONID eID) { return m_iCurrentBullet[eID]; }

private:
	CPlayer::PLAYER_DESC	m_ePlayerDesc = {};
	_uint m_eWeaponDesc[WEAPON_END] = {};
	_uint m_iMaxBullet[WEAPON_END] = { 0, 80, 60, 90 };
	_uint m_iCurrentBullet[WEAPON_END] = {8, 8, 1, 30};

	class CGameInstance* m_pGameInstance = { nullptr };

public:
	static CData_Manager* Create();
	virtual void Free() override;
};

END