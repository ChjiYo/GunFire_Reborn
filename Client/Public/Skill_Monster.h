#pragma once

#include "Skill.h"

class CSkill_Monster abstract :public CSkill
{
protected:
	CSkill_Monster(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CSkill_Monster(const CSkill& Prototype);
	virtual ~CSkill_Monster() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free() override;
};

