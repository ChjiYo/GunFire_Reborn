#pragma once

#include "Skill_Monster.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CVIBuffer_Rect;
class CCollider;
END

class CVFX_Mon_PosCircle final :public CSkill_Monster
{
protected:
	CVFX_Mon_PosCircle(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CVFX_Mon_PosCircle(const CVFX_Mon_PosCircle& Prototype);
	virtual ~CVFX_Mon_PosCircle() = default;

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

public:
	static CVFX_Mon_PosCircle* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

