#pragma once

#include "Element.h"

BEGIN(Engine)
class CShader;
class CModel;
class CBinModel;
class CCollider;
END


BEGIN(Client)

class CWall_102 final : public CElement
{
private:
	CWall_102(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CWall_102(const CWall_102& pPrototype);
	virtual ~CWall_102() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta);
	virtual void Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render();

public:
	CCollider* Get_Collider() { return m_pColliderCom; }

private:
	CShader* m_pShaderCom = { nullptr };
	CBinModel* m_pModelCom = { nullptr };
	CCollider* m_pColliderCom = { nullptr };

private:
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();

public:
	static CWall_102* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END