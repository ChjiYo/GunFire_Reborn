#pragma once

#include "Client_Defines.h"
#include "Monster.h"

BEGIN(Engine)
class CShader;
class CBinModel;
class CState_Machine;
END

BEGIN(Client)

class CMon_Spear final : public CMonster
{
private:
	CMon_Spear(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CMon_Spear(const CMon_Spear& Prototype);
	virtual ~CMon_Spear() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	void Destroy();

private:
	CShader* m_pShaderCom = { nullptr };
	CBinModel* m_pModelCom = { nullptr };
	CState_Machine* m_pStateCom = { nullptr };

private:
	_uint m_iAnimFrame = { 0 };

private:
	HRESULT Ready_State();
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();

public:
	static CMon_Spear* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END