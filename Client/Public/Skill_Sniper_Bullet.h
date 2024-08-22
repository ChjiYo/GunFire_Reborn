#pragma once

#include "Skill_Player.h"

class CSkill_Sniper_Bullet final :public CSkill_Player
{
protected:
	CSkill_Sniper_Bullet(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CSkill_Sniper_Bullet(const CSkill& Prototype);
	virtual ~CSkill_Sniper_Bullet() = default;

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
	void Destroy(_float fTimeDelta);

private:
	_vector m_vTargetLook = {};

	_float m_fDestroyTimer = {};
	_uint m_iTextureIndex = { 0 };
public:
	static CSkill_Sniper_Bullet* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

