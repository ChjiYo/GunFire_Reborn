#pragma once

#include "Element.h"

BEGIN(Client)

class CWall_Stage1 final : public CElement
{
private:
	CWall_Stage1(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CWall_Stage1(const CWall_Stage1& pPrototype);
	virtual ~CWall_Stage1() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta);
	virtual void Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render();

public:
	static CWall_Stage1* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END