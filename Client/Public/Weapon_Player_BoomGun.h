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

class CWeapon_Player_BoomGun final : public CWeapon_Player
{
private:
	CWeapon_Player_BoomGun(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CWeapon_Player_BoomGun(const CWeapon_Player_BoomGun& Prototype);
	virtual ~CWeapon_Player_BoomGun() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:

private:
	CShader* m_pShaderCom = { nullptr };
	CBinModel* m_pModelCom = { nullptr };

	CCollider* m_pColliderCom = { nullptr };

private:
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();

	void Key_State();
	void Create_Bullet();

public:
	static CWeapon_Player_BoomGun* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END

