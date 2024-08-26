#pragma once

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CCollider final : public CComponent
{
public:
	enum TYPE { TYPE_AABB , TYPE_OBB , TYPE_SPHERE , TYPE_RAY , TYPE_END};

private:
	CCollider(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CCollider(const CCollider& Prototype);
	virtual ~CCollider() = default;

public:
	virtual HRESULT Initialize_Prototype(TYPE eColliderType);
	virtual HRESULT Initialize(void* pArg);
	virtual void Update(_fmatrix TransformMatrix);
	virtual HRESULT Render() override;

public:
	_bool		isCollision(CCollider* pTargetCollider);
	class CGameObject* Get_Object() { return m_pObject; }
	class CBounding* Get_Bounding() { return m_pBounding; }
	
private:
	TYPE		m_eColliderType = { TYPE_END };
	class CBounding* m_pBounding = { nullptr };
	_bool					m_isColl = { false };

#ifdef _DEBUG
private:
	PrimitiveBatch<VertexPositionColor>* m_pBatch = { nullptr };
	BasicEffect* m_pEffect = { nullptr };
	ID3D11InputLayout* m_pInputLayout = { nullptr };
#endif

private:
	class CGameObject* m_pObject = {};

public:
	static CCollider* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, TYPE eColliderType);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};


END

