#pragma once

#include "UIObject.h"
#include "Monster.h"

BEGIN(Client)

class CUI_NPC_Weapon_Info final :public CUIObject
{
private:
	CUI_NPC_Weapon_Info(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_NPC_Weapon_Info(const CUI_NPC_Weapon_Info& Prototype);
	virtual ~CUI_NPC_Weapon_Info() = default;

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
	static CUI_NPC_Weapon_Info* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END