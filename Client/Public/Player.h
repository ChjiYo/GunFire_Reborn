#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CShader;
class CModel;
class CBinModel;
class CKeyState;
class CState_Machine;
END

BEGIN(Client)

class CPlayer final : public CGameObject
{
public:
	typedef struct :public CTransform::TRANSFORM_DESC
	{
		_int iHp = { 0 };
		_int iShield = { 0 };

		_int iMaxHp = { 0 };
		_int iMaxShield = { 0 };

		CTransform* pTargetTransform = { nullptr };
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

private:
	CShader* m_pShaderCom = { nullptr };
	CModel* m_pModelCom = { nullptr };
	CState_Machine* m_pState = { nullptr };
	CTransform* m_pCamTransform = { nullptr };

private:
	PLAYER_DESC Player_Desc = {};
	_uint m_pAnimFrame = { 0 };

private:
	HRESULT Ready_Components();
	HRESULT Ready_State();
	HRESULT Bind_ShaderResources();

	void Key_State();
	void Position();

public:
	static CPlayer* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END

