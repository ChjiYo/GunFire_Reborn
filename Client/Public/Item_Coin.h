#pragma once

#include "Item_Weapon.h"

BEGIN(Engine)
class CShader;
class CModel;
class CBinModel;
class CCollider;
class CNavigation;
END


BEGIN(Client)

class CItem_Coin final : public CItem_Weapon
{
private:
	CItem_Coin(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CItem_Coin(const CItem_Coin& pPrototype);
	virtual ~CItem_Coin() = default;

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
	CCollider* m_pCollider = { nullptr };
	CNavigation* m_pNavigationCom = { nullptr };

private:
	_bool	m_bLand = { false };

private:
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();

public:
	static CItem_Coin* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END