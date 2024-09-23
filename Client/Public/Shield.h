#pragma once

#include "Client_Defines.h"
#include "Weapon_Player.h"

BEGIN(Engine)

class CShader;
class CCollider;

END

BEGIN(Client)

class CShield final : public CPartObject
{
public:
	typedef struct : CPartObject::PARTOBJECT_DESC
	{
		const _uint* pParentState;
		const _float4x4* pSocketBoneMatrix;
	}WEAPON_DESC;

private:
	CShield(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CShield(const CShield& Prototype);
	virtual ~CShield() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();

private:
	CShader* m_pShaderCom = { nullptr };
	CCollider* m_pColliderCom = { nullptr };

	const _uint* m_pParentState = { nullptr };
	const _float4x4* m_pSocketMatrix = { nullptr };

public:
	static CShield* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END


