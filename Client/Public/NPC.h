#pragma once

#include "Client_Defines.h"
#include "ContainerObject.h"
#include "GameObject.h"

BEGIN(Client)

class CNPC abstract : public CGameObject
{
protected:
	CNPC(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CNPC(const CNPC& Prototype);
	virtual ~CNPC() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	HRESULT Ready_Components();

public:
	virtual void Free() override;
};

END