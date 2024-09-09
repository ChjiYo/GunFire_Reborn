#pragma once

#include "Client_Defines.h"
#include "Weapon_Player.h"

#include <deque>

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
	_matrix Get_EffectPos() { 
		return m_StackPos.front(); 
	}

	_uint Get_PosSIze() {
		return m_StackPos.size();
	}

private:
	CShader* m_pShaderCom = { nullptr };
	CBinModel* m_pModelCom = { nullptr };

	CCollider* m_pColliderCom = { nullptr };
	CTransform* m_pPlayerTransform = { nullptr };

	_bool					m_isShadow = { true };
	_uint					m_iPassIndex = { 0 };

private: 
	deque<_matrix> m_StackPos = {};

	_float m_fPosSaveTimer = { 0.f };
	_float m_fMoveSkillTimer = { 0.f };
	_bool	m_bBackPos = { false };

private:
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();

	void Key_State();
	void Create_Bullet();
	void PositionSave();

public:
	static CWeapon_Player_Pistol* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END

