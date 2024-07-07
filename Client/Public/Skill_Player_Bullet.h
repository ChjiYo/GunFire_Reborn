#pragma once

#include "Skill_Player.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CVIBuffer_Rect;
END

class CSkill_Player_Bullet final :public CSkill_Player
{
protected:
	CSkill_Player_Bullet(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CSkill_Player_Bullet(const CSkill& Prototype);
	virtual ~CSkill_Player_Bullet() = default;

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

private:
	_vector m_vCamLook = {};
	_float m_fTimer = {};

public:
	static CSkill_Player_Bullet* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) ;
	virtual void Free() override;
};

