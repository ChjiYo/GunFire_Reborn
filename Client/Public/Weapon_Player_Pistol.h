#pragma once

#include "Client_Defines.h"
#include "Weapon_Player.h"

BEGIN(Engine)
class CShader;
class CModel;
class CBinModel;
class CKeyState;
class CCollider;
class CState_Machine;
END

BEGIN(Client)

class CWeapon_Player_Pistol final : public CWeapon_Player
{
public:
	typedef struct :public CWeapon_Player::PLAYER_WEAPON_DESC
	{
		_uint iNumBullet = {};
	}WEAPON_DESC;

private:
	CWeapon_Player_Pistol(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CWeapon_Player_Pistol(const CWeapon_Player_Pistol& Prototype);
	virtual ~CWeapon_Player_Pistol() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	WEAPON_DESC Get_Player_Desc() { return Player_Desc; }

private:
	CShader* m_pShaderCom = { nullptr };
	CBinModel* m_pModelCom = { nullptr };

	CCollider* m_pColliderCom = { nullptr };

private:
	WEAPON_DESC			Player_Desc = {};
	_uint				m_pAnimFrame = { 0 };

private:
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();

	void Key_State();
	void Create_Bullet();

public:
	static CWeapon_Player_Pistol* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END

