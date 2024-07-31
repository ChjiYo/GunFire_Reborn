#pragma once

#include "Client_Defines.h"
#include "PartObject.h"

BEGIN(Client)

class CWeapon_Player abstract :public CPartObject
{
public:
	typedef struct :public CPartObject::PARTOBJECT_DESC
	{
		const _uint* pParentState;
		CTransform* pTargetTransform = { nullptr };

		_uint iNumBullet = {};
		_float fAttackSpeed = {};
	}PLAYER_WEAPON_DESC;

protected:
	CWeapon_Player(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CWeapon_Player(const CWeapon_Player& Prototype);
	virtual ~CWeapon_Player() = default;


public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta)	override;
	virtual void Late_Update(_float fTimeDelta)	override;
	virtual HRESULT Render()	override;

protected:
	_uint m_iNumBullet = { 0 };
	_uint m_iNumMaxBullet = { 0 };
	_float m_fAttackSpeed = { 0.f };
	_float m_fAttackTimer = { 0.f };

public:
	virtual void Free() override;
};

END