#include "..\Public\CustomFont.h"

#include "GameInstance.h"

CCustomFont::CCustomFont(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: m_pDevice { pDevice }
	, m_pContext { pContext }
	, m_pGameInstance{ CGameInstance::Get_Instance() }
{
	Safe_AddRef(m_pGameInstance);
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

HRESULT CCustomFont::Initialize(const _tchar * pFontFilePath)
{
	m_pFont = new SpriteFont(m_pDevice, pFontFilePath);
	m_pBatch = new SpriteBatch(m_pContext);

	D3D11_BLEND_DESC			BlendDesc{};
	BlendDesc.RenderTarget[0].BlendEnable = true;
	BlendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	BlendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	BlendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	BlendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	BlendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	BlendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	BlendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_RED | D3D11_COLOR_WRITE_ENABLE_GREEN | D3D11_COLOR_WRITE_ENABLE_BLUE;

	m_pDevice->CreateBlendState(&BlendDesc, &m_pBlendState);

	return S_OK;
}

HRESULT CCustomFont::Draw(const _tchar* pText, const _float2& vPosition, _fvector vFontColor, _float fRadian, _float2 vPivotPos, _float fScale)
{	
	m_pContext->GSSetShader(nullptr, nullptr, 0);

	m_pBatch->Begin(DirectX::SpriteSortMode_Deferred, m_pBlendState);

	m_pFont->DrawString(m_pBatch, pText, vPosition, vFontColor, fRadian, vPivotPos, fScale);

	m_pBatch->End();

	return S_OK;
}

HRESULT CCustomFont::DeathDraw(const _tchar* pText, _vector vPosition, _fvector vFontColor, _float fRadian, _vector vPivotPos, _vector fScale)
{
	
	_matrix matView = m_pGameInstance->Get_Transform_Matrix(CPipeLine::D3DTS_VIEW);
	_matrix matProj = m_pGameInstance->Get_Transform_Matrix(CPipeLine::D3DTS_PROJ);

	// SpriteBatch 시작
	m_pContext->GSSetShader(nullptr, nullptr, 0);
	m_pBatch->Begin(SpriteSortMode_Deferred, m_pBlendState, nullptr, nullptr, nullptr, nullptr, matView * matProj);

	// 3D 위치 설정 (vPosition을 XMVECTOR로 변환)
	XMVECTOR position = XMVectorSet(0.f, 0.f, 5.f, 1.0f);

	// 3D 텍스트 그리기
	XMVECTOR origin = XMVectorSet(0.f, 0.f, 5.0f, 1.0f);
	XMVECTOR scale = XMVectorSet(2.f, 2.f,0.0f, 0.0f);
	m_pFont->DrawString(m_pBatch, pText, position, vFontColor, fRadian, origin, scale, SpriteEffects_None , 1.f);

	// SpriteBatch 종료
	m_pBatch->End();

	return S_OK;
}

CCustomFont * CCustomFont::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, const _tchar * pFontFilePath)
{
	CCustomFont*		pInstance = new CCustomFont(pDevice, pContext);

	if (FAILED(pInstance->Initialize(pFontFilePath)))
	{
		MSG_BOX(TEXT("Failed to Created : CCustomFont"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


void CCustomFont::Free()
{
	Safe_Release(m_pGameInstance);

	Safe_Delete(m_pBatch);
	Safe_Delete(m_pFont);

	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
	Safe_Release(m_pBlendState);

	__super::Free();
}
