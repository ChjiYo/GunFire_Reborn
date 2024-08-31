#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

#include "Terrain.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CVIBuffer_Terrain;
class CNavigation;
END

BEGIN(Client)

class CTerrain_Boss final : public CTerrain
{
private:
	CTerrain_Boss(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CTerrain_Boss(const CTerrain_Boss& Prototype);
	virtual ~CTerrain_Boss() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	HRESULT Ready_Components();

public:
	static CTerrain_Boss* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END