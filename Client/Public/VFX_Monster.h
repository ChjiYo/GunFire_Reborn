#pragma once

#include "Client_Defines.h"
#include "Effect.h"

BEGIN(Engine)
class CNavigation;
END

BEGIN(Client)

class CVFX_Monster abstract :public CEffect
{
public:
	typedef struct :public CPartObject::PARTOBJECT_DESC
	{
		CTransform* pOriginTransform = {};
	}MON_VFX_DESC;

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
	MON_VFX_DESC	m_tVFX_Desc = {};

public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free() override;

};

END

