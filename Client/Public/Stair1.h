#pragma once

#include "Element.h"

BEGIN(Engine)
class CShader;
class CModel;
class CBinModel;
END


BEGIN(Client)

class CStair1 final : public CElement
{
private:
	CStair1(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CStair1(const CStair1& pPrototype);
	virtual ~CStair1() = default;

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
	static CStair1* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END