#pragma once

#include "UIObject.h"
#include "Monster.h"

BEGIN(Client)

class CUI_MiniMap final :public CUIObject
{
private:
	CUI_MiniMap(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_MiniMap(const CUI_MiniMap& Prototype);
	virtual ~CUI_MiniMap() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta);
	virtual void Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render();

private:
	virtual HRESULT Ready_Components();

public:
	static CUI_MiniMap* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END