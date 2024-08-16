#pragma once

#include "Client_Defines.h"
#include "Monster.h"

BEGIN(Engine)
class CShader;
class CBinModel;
class CState_Machine;
END

BEGIN(Client)

class CMon_Deash_Boom final : public CMonster
{
private:
	CMon_Deash_Boom(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CMon_Deash_Boom(const CMon_Deash_Boom& Prototype);
	virtual ~CMon_Deash_Boom() = default;

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
	static CMon_Deash_Boom* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END