#pragma once

#include "Element.h"

BEGIN(Engine)
class CShader;
class CModel;
class CBinModel;
END


BEGIN(Client)

class CStair101 final : public CElement
{
private:
	CStair101(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CStair101(const CStair101& pPrototype);
	virtual ~CStair101() = default;

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
	HRESULT Ready_Components();
	HRESULT Bind_ShaderResources();

public:
	static CStair101* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END