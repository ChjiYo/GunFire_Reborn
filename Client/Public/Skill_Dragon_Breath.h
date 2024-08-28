#pragma once

#include "Skill_Monster.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CVIBuffer_Rect;
class CCollider;
END

class CSkill_Dragon_Breath final :public CSkill_Monster
{
protected:
	CSkill_Dragon_Breath(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CSkill_Dragon_Breath(const CSkill_Dragon_Breath& Prototype);
	virtual ~CSkill_Dragon_Breath() = default;

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
	_bool IsCollider(_float fOriginX , _float fOriginZ ,_float fTargetX , _float fTargetZ , _float fDistance);

private:
	CShader* m_pShaderCom = { nullptr };
	CTexture* m_pTextureCom = { nullptr };
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };
	CCollider* m_pColliderCom = { nullptr };

private:
	_float m_fDestroyTimer = {};
	_float m_fTickDamageTimer = { 0.f };

	_float m_fAngle = { 0.f };
	_float m_fAnimFrame = { 0.f };

public:
	static CSkill_Dragon_Breath* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

