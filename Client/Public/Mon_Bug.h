#pragma once

#include "Client_Defines.h"
#include "Monster.h"

BEGIN(Engine)
class CShader;
class CBinModel;
class CState_Machine;
END

BEGIN(Client)

class CMon_Bug final : public CMonster
{
private:
	CMon_Bug(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CMon_Bug(const CMon_Bug& Prototype);
	virtual ~CMon_Bug() = default;

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
	_uint m_iAnimFrame = { 0 };
	_bool m_bDeadEffect = { false };

private:
	HRESULT Ready_State();
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();

public:
	static CMon_Bug* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END