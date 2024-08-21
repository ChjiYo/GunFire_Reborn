#pragma once

#include "Element.h"

BEGIN(Engine)
class CShader;
class CModel;
class CBinModel;
END


BEGIN(Client)

class CStair2 final : public CElement
{
private:
	CStair2(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CStair2(const CStair2& pPrototype);
	virtual ~CStair2() = default;

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
	static CStair2* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override;
};

END