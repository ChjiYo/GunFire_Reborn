#pragma once

#include "Element.h"

BEGIN(Engine)
class CShader;
class CBinModel;
class CCollider;
END


BEGIN(Client)

class CBox final : public CElement
{
private:
	CBox(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CBox(const CBox& pPrototype);
	virtual ~CBox() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta);
	virtual void Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render();

private:
	CShader* m_pShaderCom = { nullptr };
	CBinModel* m_pModelCom = { nullptr };
	CCollider* m_pColliderCom = { nullptr };

private:
	_bool m_bItemCreate = {false};
	_bool m_bAnimStop = { true };
	_float m_fItemCreateTimer = { 0.f };

private:
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();

public:
	static CBox* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END