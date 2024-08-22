#pragma once

#include "Client_Defines.h"
#include "Item.h"

BEGIN(Client)

class CItem_Weapon abstract : public CItem
{
public:
	enum ITEMID { PISTOL, SNIPER, SHURIKEN, BOOMGUN, WEAPON_END };

protected:
	CItem_Weapon(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CItem_Weapon(const CItem_Weapon& pPrototype);
	virtual ~CItem_Weapon() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta);
	virtual void Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render();

protected:
	ITEMID m_eWeaponID = { WEAPON_END };

public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free() override;
};

END