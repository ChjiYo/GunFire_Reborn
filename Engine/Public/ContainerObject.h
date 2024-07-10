#pragma once

#include "GameObject.h"

class CContainerObject abstract:public CGameObject
{
public:
	typedef struct : public CGameObject::GAMEOBJECT_DESC
	{
		_uint	iNumParts;
	}CONTAINEROBJECT_DESC;

protected:
	CContainerObject(ID3D11Device* pDevice , ID3D11DeviceContext* pContext);
	CContainerObject(const CContainerObject& pPrototype);
	virtual ~CContainerObject() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta);
	virtual void Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render();

protected:
	_uint								m_iNumParts = {};
	vector<class CPartObject*>			m_Parts;

protected:
	HRESULT Add_PartObject(_uint iPartIndex, const _wstring& strPrototypeTag, void* pArg);

public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free() override;
};

