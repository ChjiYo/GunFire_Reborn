#pragma once

#include "Client_Defines.h"
#include "Effect.h"

BEGIN(Engine)
class CNavigation;
END

BEGIN(Client)

class CVFX_Monster abstract :public CEffect
{
protected:
	CVFX_Monster(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CVFX_Monster(const CVFX_Monster& Prototype);
	virtual ~CVFX_Monster() = default;

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

public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free() override;

};

END

