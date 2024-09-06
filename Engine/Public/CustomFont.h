#pragma once

#include "Base.h"

/* 폰트 파일하나를 빌드한 객체 */
BEGIN(Engine)

class CCustomFont final : public CBase
{
private:
	CCustomFont(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CCustomFont() = default;

public:
	HRESULT Initialize(const _tchar* pFontFilePath);
	HRESULT Draw(const _tchar* pText, const _float2& vPosition, _fvector vFontColor, _float fRadian, _float2 vPivotPos, _float fScale);
	HRESULT DeathDraw(const _tchar* pText, _vector vPosition, _fvector vFontColor, _float fRadian, _vector  vPivotPos, _vector fScale);
	
private:
	ID3D11Device*				m_pDevice = { nullptr };
	ID3D11DeviceContext*		m_pContext = { nullptr };
	ID3D11BlendState*			m_pBlendState = { nullptr };

	class CGameInstance*			m_pGameInstance = { nullptr };

private:
	SpriteBatch*				m_pBatch = { nullptr };
	SpriteFont*					m_pFont = { nullptr };


public:
	static CCustomFont* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _tchar* pFontFilePath);
	virtual void Free() override;
};

END