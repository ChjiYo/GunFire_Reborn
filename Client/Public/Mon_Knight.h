 #pragma once

#include "Client_Defines.h"
#include "Monster.h"

BEGIN(Engine)
class CShader;
class CBinModel;
class CState_Machine;
class CTexture;
END

BEGIN(Client)

class CMon_Knight final : public CMonster
{
private:
	CMon_Knight(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CMon_Knight(const CMon_Knight& Prototype);
	virtual ~CMon_Knight() = default;

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
	CTexture* m_pTextureCom = { nullptr };
private:
	_float m_fTimer  = { 0 };
	_bool m_bNoise = {false};

private:
	HRESULT Ready_State();
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();

public:
	static CMon_Knight* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END