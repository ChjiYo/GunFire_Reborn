#pragma once

#include "Client_Defines.h"
#include "Monster.h"

BEGIN(Engine)
class CShader;
class CBinModel;
class CState_Machine;
END

BEGIN(Client)

class CMon_Dragon final : public CMonster
{
private:
	CMon_Dragon(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CMon_Dragon(const CMon_Dragon& Prototype);
	virtual ~CMon_Dragon() = default;

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


	_float m_fEffectCreateTimer = { 0.f };

private:
	HRESULT Ready_State();
	HRESULT Ready_Components();
	HRESULT Ready_UI();
	HRESULT Bind_ShaderResources();

public:
	static CMon_Dragon* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END