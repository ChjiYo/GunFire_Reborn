#pragma once

#include "Client_Defines.h"
#include "NPC.h"

BEGIN(Engine)

class CShader;
class CBinModel;
class CCollider;

END

BEGIN(Client)

class CNPC_Weapon final : public CNPC
{
protected:
	CNPC_Weapon(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CNPC_Weapon(const CNPC_Weapon& Prototype);
	virtual ~CNPC_Weapon() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();

public:
	_bool GetCursor() { return m_bCursorFree; }

private:
	CShader* m_pShaderCom = { nullptr };
	CBinModel* m_pModelCom = { nullptr };
	CCollider* m_pColliderCom = { nullptr };

private:
	_bool m_bClick = { false };
	_bool m_bCursorFree = { false };
	
public:
	static CNPC_Weapon* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END