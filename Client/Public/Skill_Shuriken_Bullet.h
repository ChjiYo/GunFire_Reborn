#pragma once

#include "Skill_Player.h"

BEGIN(Engine)

class CNavigation;

END

BEGIN(Client)

class CSkill_Shuriken_Bullet final :public CSkill_Player
{

protected:
	CSkill_Shuriken_Bullet(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CSkill_Shuriken_Bullet(const CSkill& Prototype);
	virtual ~CSkill_Shuriken_Bullet() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();

private:
	void Destroy(_float fTimeDelta, _vector vPos);
	void NaviReflect();
	void HighReflect();

private:
	class CNavigation* m_pNaviCom = { nullptr };

private:
	_float m_fReCollTimer = { 0.f };

	_bool m_bWallColl = { false };

	_vector m_vTargetLook = {};
	_vector m_vReflectVec = {};

	_float m_fDestroyTimer = {};
	_uint m_iTextureIndex = { 0 };

	_float4 m_vDestroyPos = {};

	_uint m_iNumBounding = { 0 };

public:
	static CSkill_Shuriken_Bullet* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END