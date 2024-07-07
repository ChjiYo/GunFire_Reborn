#pragma once

#include "Base.h"

/* 객체들을 리스트로 보관한다. */

BEGIN(Engine)

class CLayer final : public CBase
{
private:
	CLayer();
	virtual ~CLayer() = default;

public:
	void Priority_Update(_float fTimeDelta);
	void Update(_float fTimeDelta);
	void Late_Update(_float fTimeDelta);

public:
	class CGameObject* Get_GameObjectByIndex(_uint iIndex);
	HRESULT Add_List(list<class CGameObject*>* pList);
	HRESULT Add_GameObject(class CGameObject* pGameObject);

	class CComponent* Get_Component(const wstring& strComponentTag, _uint iIndex);
	list<class CGameObject*> Get_Object_List() { return m_GameObjects; }

private:
	list<class CGameObject*>			m_GameObjects;

public:
	static CLayer* Create();
	virtual void Free() override;
};

END