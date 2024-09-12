#pragma once

#include "UIObject.h"
#include "Monster.h"

BEGIN(Client)

class CUI_Coin final :public CUIObject
{
private:
	CUI_Coin(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_Coin(const CUI_Coin& Prototype);
	virtual ~CUI_Coin() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta);
	virtual void Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render();

public:
	void Set_Coin(_int iCoin) { m_iCopyCoin += iCoin; m_bAddCoin = true; }

private:
	virtual HRESULT Ready_Components();

private:
	_int m_iCoin = { 0 };
	_int m_iCopyCoin = { 0 };

	_bool m_bAddCoin = {false};
	
public:
	static CUI_Coin* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END