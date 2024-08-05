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

protected:
	_float Get_RandomNormalize();
	_float Get_Random(_float fMin, _float fMax);

	void Offset_Rotation(_vector vOffsetRandomPos);

protected:
	_vector		m_vPlayerPos = {};

public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free() override;
};

