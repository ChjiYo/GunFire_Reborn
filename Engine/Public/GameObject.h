#pragma once

#include "Transform.h"	
/* 클라이언트 개발자가 제작한 객체들의 부모 클래스가 된다. */

BEGIN(Engine)

class ENGINE_DLL CGameObject abstract : public CBase
{
public:
	typedef struct : public CTransform::TRANSFORM_DESC
	{
		_uint		iGameObjectData = {};
	}GAMEOBJECT_DESC;
protected:
	CGameObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CGameObject(const CGameObject& Prototype);
	virtual ~CGameObject() = default;

public:
	class CComponent* Get_Component(const _wstring& strComponentTag) {
		auto	iter = m_Components.find(strComponentTag);
		if (iter == m_Components.end())
			return nullptr;
		return iter->second;
	}

	void SetPicking(_bool _picking) { m_bIsPicking = _picking; };
	_bool GetIsPicking() { return m_bIsPicking; };

public:
	static const _wstring		m_strTransformTag;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta);
	virtual void Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render();

public:
	HRESULT Add_Component(_uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, const _wstring& strComponentTag, class CComponent** ppOut, void* pArg = nullptr);
	void	Set_Dead() { m_bDead = true; }
	_bool	Get_Dead() { return m_bDead; }

protected:	
	ID3D11Device*				m_pDevice = { nullptr };
	ID3D11DeviceContext*		m_pContext = { nullptr };
	class CGameInstance*		m_pGameInstance = { nullptr };

	_uint						m_iGameObjectData = { 0 };

	class CTransform*			m_pTransformCom = { nullptr };

	_bool						m_bIsPicking = { false };
	_bool						m_bIsPasingObject = { false };

protected:
	map<const _wstring, CComponent*>		m_Components;
	_bool									m_bDead = { false };
	_float3									m_vPosition = {};

public:
	_uint						m_iIndex = { 0 };

public:
	virtual CGameObject* Clone(void* pArg = nullptr) = 0;
	virtual void Free() override;
};

END