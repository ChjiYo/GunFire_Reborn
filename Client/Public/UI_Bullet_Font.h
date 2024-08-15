#pragma once

#include "GameObject.h"
#include "Client_Defines.h"

BEGIN(Client)

class CUI_Bullet_Font final :public CGameObject
{
private:
	CUI_Bullet_Font(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI_Bullet_Font(const CUI_Bullet_Font& Prototype);
	virtual ~CUI_Bullet_Font() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta);
	virtual void Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render();

private:
	HRESULT Ready_Fonts();

public:
	static CUI_Bullet_Font* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END