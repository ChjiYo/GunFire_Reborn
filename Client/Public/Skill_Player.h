#pragma once

#include "Skill.h"

class CSkill_Player abstract:public CSkill
{
protected:
	CSkill_Player(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CSkill_Player(const CSkill& Prototype);
	virtual ~CSkill_Player() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

protected:
	HRESULT Ready_Components();
	void Set_Position(_float3 vOffSet);

public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free() override;
};

