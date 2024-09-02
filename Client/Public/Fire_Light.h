#pragma once

#include "Element.h"

BEGIN(Client)

class CFire_Light final : public CElement
{
private:
	CFire_Light(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CFire_Light(const CFire_Light& pPrototype);
	virtual ~CFire_Light() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta);
	virtual void Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render();

public:
	static CFire_Light* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END