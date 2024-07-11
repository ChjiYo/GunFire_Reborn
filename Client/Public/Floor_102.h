#pragma once

#include "Element.h"

BEGIN(Engine)
class CShader;
class CModel;
END


BEGIN(Client)

class CFloor_102 final : public CElement
{
private:
	CFloor_102(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CFloor_102(const CFloor_102& pPrototype);
	virtual ~CFloor_102() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta);
	virtual void Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render();

private:
	CShader* m_pShaderCom = { nullptr };
	CModel* m_pModelCom = { nullptr };

private:
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();

public:
	static CFloor_102* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END