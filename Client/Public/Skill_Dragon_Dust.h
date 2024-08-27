#pragma once

#include "Skill_Monster.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CVIBuffer_Rect;
class CCollider;
END

class CSkill_Dragon_Dust final :public CSkill_Monster
{
protected:
	CSkill_Dragon_Dust(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CSkill_Dragon_Dust(const CSkill_Dragon_Dust& Prototype);
	virtual ~CSkill_Dragon_Dust() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	HRESULT Ready_Components();
	HRESULT Ready_PartObjects();
	HRESULT Bind_ShaderResources();

private:
	void Destroy(_float fTimeDelta);
	_bool IsCollider(_vector vCollStartPos  ,_vector  vCollObjPos, _float fDamageDist);

private:
	CShader* m_pShaderCom = { nullptr };
	CTexture* m_pTextureCom = { nullptr };
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };
	CCollider* m_pColliderCom = { nullptr };

private:
	_vector		m_vPos = {};

	_vector		m_vOffsetRandomPos = {};

	_float m_fDestroyTimer = {};
	_float m_fAnimFrame = { 0.f };

	_uint m_iTextureIndex = { 0 };
	_float m_fDistance = { 0.f };

	_bool m_bAttack = {false};
public:
	static CSkill_Dragon_Dust* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

