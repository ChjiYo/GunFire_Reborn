#pragma once

#include "Base.h"

BEGIN(Engine)

class CFont_Manager final : public CBase
{
private:
	CFont_Manager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CFont_Manager() = default;

public:
	HRESULT Add_Font(ID3D11Device* pDevice, ID3D11DeviceContext* pContext , const _wstring& strFontTag, const _wstring& strFontFilePath);
	HRESULT Draw_Font(const _wstring& strFontTag, const _tchar* pText, const _float2& vPosition, _fvector vFontColor, _float fRadian, _float2 vPivotPos, _float fScale);
	HRESULT DeathDraw_Font(const _wstring& strFontTag, const _tchar* pText, _vector vPosition, _fvector vFontColor, _float fRadian, _vector vPivotPos, _vector fScale);

private:
	ID3D11Device*						m_pDevice = { nullptr };
	ID3D11DeviceContext*				m_pContext = { nullptr };

	map<const _wstring, class CCustomFont*>			m_Fonts;

private:
	class CCustomFont* Find_Font(const _wstring& strFontTag);

public:
	static CFont_Manager* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;
};

END