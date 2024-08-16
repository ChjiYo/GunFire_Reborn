#pragma once

#include "Element.h"

BEGIN(Engine)
class CShader;
class CModel;
class CBinModel;
END


BEGIN(Client)

class CDoor final : public CElement
{
private:
	CDoor(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CDoor(const CDoor& pPrototype);
	virtual ~CDoor() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta);
	virtual void Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render();

private:
	CShader* m_pShaderCom = { nullptr };
	CBinModel* m_pModelCom = { nullptr };

private:
	_uint m_iAnimFrame = {0};

private:
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();

public:
	static CDoor* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END