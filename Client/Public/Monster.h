#pragma once

#include "Client_Defines.h"
#include "ContainerObject.h"
#include "UIObject.h"

BEGIN(Engine)

class CNavigation;
class CCollider;

END

BEGIN(Client)

class CMonster abstract : public CContainerObject
{
public:
	typedef struct
	{
		_uint iHp = {};
		_uint iMaxHp = {};
	}MON_DESC;

public:
	enum COLLIDERTYPE { COLL_AABB, COLL_SPHERE, COLL_END };
	enum PARTID { PART_HEAD, PART_EFFECT, PART_END };

protected:
	CMonster(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CMonster(const CMonster& Prototype);
	virtual ~CMonster() = default;

public:
	MON_DESC Get_MonDesc() { return m_tMonDesc; }
	void Set_Damage(_uint iDamage)
	{
		m_tMonDesc.iHp += iDamage;
	}

	CCollider* Get_Collider() { return m_pColliderCom[COLL_AABB]; }

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	HRESULT Ready_Components();
	HRESULT Ready_UI();

protected:
	CNavigation* m_pNavigationCom	= { nullptr };
	CCollider*	m_pColliderCom[COLL_END] = { nullptr , nullptr };

	MON_DESC	m_tMonDesc = {};

public:
	virtual void Free() override;
};

END