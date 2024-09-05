#pragma once

#include "Item_Weapon.h"

BEGIN(Engine)
class CShader;
class CModel;
class CBinModel;
END


BEGIN(Client)

class CItem_BoomGun final : public CItem_Weapon
{
private:
	CItem_BoomGun(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CItem_BoomGun(const CItem_BoomGun& pPrototype);
	virtual ~CItem_BoomGun() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta);
	virtual void Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render();

private:
	CShader* m_pShaderCom = { nullptr };
	CBinModel* m_pModelCom = { nullptr };

private:
	_float m_fMoveTimer = { 0.f };
	_float m_fUpWeight = { 0.f };

private:
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();

public:
	static CItem_BoomGun* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END