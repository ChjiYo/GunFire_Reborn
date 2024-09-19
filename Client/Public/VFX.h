#pragma once

#include "Client_Defines.h"
#include "GameObject.h"


BEGIN(Client)

class CVFX abstract :public CGameObject
{
public:
	typedef struct :public CGameObject::GAMEOBJECT_DESC
	{
		CTransform* pOriginTransform = {};
		_vector vCreatePos = {};
	}VFX_DESC;

protected:
	CVFX(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CVFX(const CVFX& Prototype);
	virtual ~CVFX() = default;

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

