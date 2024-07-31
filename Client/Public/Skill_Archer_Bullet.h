#pragma once

#include "Skill_Monster.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CVIBuffer_Rect;
class CCollider;
END

class CSkill_Archer_Bullet final :public CSkill_Monster
{
protected:
	CSkill_Archer_Bullet(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CSkill_Archer_Bullet(const CSkill_Archer_Bullet& Prototype);
	virtual ~CSkill_Archer_Bullet() = default;

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
	CShader* m_pShaderCom = { nullptr };
	CTexture* m_pTextureCom = { nullptr };
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };
	CCollider* m_pColliderCom = { nullptr };

private:
	_vector m_vTargetLook = {};

	_float m_fDestroyTimer = {};
	_uint m_iTextureIndex = { 0 };
public:
	static CSkill_Archer_Bullet* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

