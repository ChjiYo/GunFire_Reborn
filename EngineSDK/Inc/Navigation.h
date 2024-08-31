#pragma once

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CNavigation final : public CComponent
{
public:
	typedef struct
	{
		_int		iCurrentCellIndex{ -1 };
	}NAVIGATION_DESC;
private:
	CNavigation(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CNavigation(const CNavigation& Prototype);
	virtual ~CNavigation() = default;

public:
	virtual HRESULT Initialize_Prototype(const _tchar* pNavigationDataFilePath);
	virtual HRESULT Initialize(void* pArg) override;

	virtual void Update(_fmatrix TerrainWorldMatrix);
	virtual HRESULT Render();

public:
	_bool is_Move(_fvector vPosition);
	HRESULT Add_Sell(_float3 fPoint[3]);
	HRESULT Save_Navi();
	HRESULT Save_DesertNavi();
	HRESULT Delete_Navi();
	HRESULT Clear_Navi();
	vector<class CCell*> Get_Cell() { return m_Cells; }
	
private:
	vector<class CCell*>				m_Cells;
	_int								m_iCurrentCellIndex = { -1 };

	static _float4x4					m_WorldMatrix;


#ifdef _DEBUG
private:
	class CShader*			m_pShader = { nullptr };
#endif

private:
	void Make_Neighbors();
public:
	_uint Init_Index(class CTransform* pTransform);
	_float Navi_Height(class CTransform* pTransform);

public:
	static CNavigation* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _tchar* pNavigationDataFilePath);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END