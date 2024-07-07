#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

class CSkill abstract:public CGameObject
{
public:
	typedef struct : public CTransform::TRANSFORM_DESC
	{
		CTransform* pPlayerTransform = {};
		CTransform* pCamTransform = {};
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
	CTransform* m_pPlayerTransform = {};
	CTransform* m_pCamTransform = {};

public:
	virtual CGameObject* Clone(void* pArg)  = 0;
	virtual void Free() override;

};

