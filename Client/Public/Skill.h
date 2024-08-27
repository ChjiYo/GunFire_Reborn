#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CNavigation;
END

BEGIN(Client)

class CSkill abstract:public CGameObject
{
public:
	typedef struct : public CGameObject::GAMEOBJECT_DESC
	{
		CTransform* pOriginTransform = {};
		_uint		iSkillDamage = {};
		_float4		vDestoryPos = {};
		_int		iNumSkill = {};
	}SKILL_DESC;
	
protected:
	CSkill(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CSkill(const CSkill& Prototype);
	virtual ~CSkill() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

protected:
	HRESULT Ready_Components();

protected:
	CTransform* m_pOriginTransform = {};
	CNavigation* m_pNavigationCom = {};

	_uint		m_iSkillDamage = {};

public:
	virtual CGameObject* Clone(void* pArg)  = 0;
	virtual void Free() override;

};

END

