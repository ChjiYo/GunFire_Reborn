#pragma once

#include "Item_Weapon.h"

BEGIN(Engine)
class CShader;
class CModel;
class CBinModel;
END


BEGIN(Client)

class CItem_Sniper final : public CItem_Weapon
{
private:
	CItem_Sniper(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CItem_Sniper(const CItem_Sniper& pPrototype);
	virtual ~CItem_Sniper() = default;

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
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();

public:
	static CItem_Sniper* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END