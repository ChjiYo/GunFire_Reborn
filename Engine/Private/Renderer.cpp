#include "..\Public\Renderer.h"

#include "GameObject.h"
#include "GameInstance.h"

#include "Shader.h"
#include "VIBuffer_Rect.h"

_uint		g_iSizeX = 10240;
_uint		g_iSizeY = 5760;

CRenderer::CRenderer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: m_pDevice{ pDevice }
	, m_pContext{ pContext }
	, m_pGameInstance{ CGameInstance::Get_Instance() }
{
	Safe_AddRef(m_pGameInstance);
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

HRESULT CRenderer::Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	D3D11_VIEWPORT			ViewportDesc{};
	_uint					iNumViewport = { 1 };
	m_pContext->RSGetViewports(&iNumViewport, &ViewportDesc);

	/* 렌더타겟들을 추가한다. */
	if (FAILED(m_pGameInstance->Add_RenderTarget(TEXT("Target_Diffuse"), ViewportDesc.Width, ViewportDesc.Height, DXGI_FORMAT_B8G8R8A8_UNORM, XMVectorSet(1.f, 1.f, 1.f, 0.f))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_RenderTarget(TEXT("Target_Normal"), ViewportDesc.Width, ViewportDesc.Height, DXGI_FORMAT_R16G16B16A16_UNORM, XMVectorSet(1.f, 1.f, 1.f, 1.f))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_RenderTarget(TEXT("Target_Depth"), ViewportDesc.Width, ViewportDesc.Height, DXGI_FORMAT_R32G32B32A32_FLOAT, XMVectorSet(1.f, 1.f, 1.f, 1.f))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_RenderTarget(TEXT("Target_PickDepth"), ViewportDesc.Width, ViewportDesc.Height, DXGI_FORMAT_R32G32B32A32_FLOAT, XMVectorSet(1.f, 1.f, 1.f, 1.f))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_RenderTarget(TEXT("Target_Shade"), ViewportDesc.Width, ViewportDesc.Height, DXGI_FORMAT_R16G16B16A16_UNORM, XMVectorSet(0.f, 0.f, 0.f, 1.f))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_RenderTarget(TEXT("Target_Specular"), ViewportDesc.Width, ViewportDesc.Height, DXGI_FORMAT_R16G16B16A16_UNORM, XMVectorSet(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_RenderTarget(TEXT("Target_LightDepth"), g_iSizeX, g_iSizeY, DXGI_FORMAT_R32G32B32A32_FLOAT, XMVectorSet(1.f, 1.f, 1.f, 1.f))))
		return E_FAIL;



	/* 멀티렌더타겟에 타겟들을 모아놓는다. */
	if (FAILED(m_pGameInstance->Add_MRT(TEXT("MRT_GameObjects"), TEXT("Target_Diffuse"))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_MRT(TEXT("MRT_GameObjects"), TEXT("Target_Normal"))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_MRT(TEXT("MRT_GameObjects"), TEXT("Target_Depth"))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_MRT(TEXT("MRT_GameObjects"), TEXT("Target_PickDepth"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_MRT(TEXT("MRT_LightAcc"), TEXT("Target_Shade"))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Add_MRT(TEXT("MRT_LightAcc"), TEXT("Target_Specular"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_MRT(TEXT("MRT_ShadowObjects"), TEXT("Target_LightDepth"))))
		return E_FAIL;
	

	//if (FAILED(m_pGameInstance->Add_RenderTarget(TEXT("Target_Blur_X"), ViewportDesc.Width, ViewportDesc.Height, DXGI_FORMAT_B8G8R8A8_UNORM, XMVectorSet(1.f, 1.f, 1.f, 0.f))))
	//	return E_FAIL;
	//if (FAILED(m_pGameInstance->Add_MRT(TEXT("MRT_Blur_X"), TEXT("Target_Blur_X"))))
	//	return E_FAIL;
	//
	//if (FAILED(m_pGameInstance->Add_RenderTarget(TEXT("Target_Blur_Y"), ViewportDesc.Width, ViewportDesc.Height, DXGI_FORMAT_B8G8R8A8_UNORM, XMVectorSet(1.f, 1.f, 1.f, 0.f))))
	//	return E_FAIL;
	//if (FAILED(m_pGameInstance->Add_MRT(TEXT("MRT_Blur_Y"), TEXT("Target_Blur_Y"))))
	//	return E_FAIL;
	//
	//if (FAILED(m_pGameInstance->Add_RenderTarget(TEXT("Target_Result"), ViewportDesc.Width, ViewportDesc.Height, DXGI_FORMAT_B8G8R8A8_UNORM, XMVectorSet(1.f, 1.f, 1.f, 0.f))))
	//	return E_FAIL;
	//if (FAILED(m_pGameInstance->Add_MRT(TEXT("MRT_Result"), TEXT("Target_Result"))))
	//	return E_FAIL;


	XMStoreFloat4x4(&m_WorldMatrix, XMMatrixScaling(ViewportDesc.Width, ViewportDesc.Height, 1.f));
	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(ViewportDesc.Width, ViewportDesc.Height, 0.f, 1.f));

	m_pVIBuffer = CVIBuffer_Rect::Create(m_pDevice, m_pContext);
	if (nullptr == m_pVIBuffer)
		return E_FAIL;

	m_pShader = CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_Deferred.hlsl"), VTXPOSTEX::Elements, VTXPOSTEX::iNumElements);
	if (nullptr == m_pShader)
		return E_FAIL;

	ID3D11Texture2D* pDepthStencilTexture = nullptr;



	D3D11_TEXTURE2D_DESC	TextureDesc;
	ZeroMemory(&TextureDesc, sizeof(D3D11_TEXTURE2D_DESC));

	TextureDesc.Width = g_iSizeX;
	TextureDesc.Height = g_iSizeY;
	TextureDesc.MipLevels = 1;
	TextureDesc.ArraySize = 1;
	TextureDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

	TextureDesc.SampleDesc.Quality = 0;
	TextureDesc.SampleDesc.Count = 1;

	TextureDesc.Usage = D3D11_USAGE_DEFAULT;
	TextureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL
		/*| D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE*/;
	TextureDesc.CPUAccessFlags = 0;
	TextureDesc.MiscFlags = 0;

	if (FAILED(m_pDevice->CreateTexture2D(&TextureDesc, nullptr, &pDepthStencilTexture)))
		return E_FAIL;

	/* RenderTarget */
	/* ShaderResource */
	/* DepthStencil */

	if (FAILED(m_pDevice->CreateDepthStencilView(pDepthStencilTexture, nullptr, &m_pShadowDSV)))
		return E_FAIL;

	Safe_Release(pDepthStencilTexture);

#ifdef _DEBUG
	if (FAILED(m_pGameInstance->Ready_RT_Debug(TEXT("Target_Diffuse"), 100.f, 100.f, 200.0f, 200.0f)))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Ready_RT_Debug(TEXT("Target_Normal"), 100.f, 300.f, 200.0f, 200.0f)))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Ready_RT_Debug(TEXT("Target_Depth"), 100.f, 500.f, 200.0f, 200.0f)))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Ready_RT_Debug(TEXT("Target_PickDepth"), 300.f, 100.f, 200.0f, 200.0f)))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Ready_RT_Debug(TEXT("Target_Shade"), 300.f, 300.f, 200.f, 200.f)))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Ready_RT_Debug(TEXT("Target_Specular"), 300.f, 500.f, 200.f, 200.f)))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Ready_RT_Debug(TEXT("Target_LightDepth"), 1600.0f - 100.0f, 100.f, 200.f, 200.f)))
		return E_FAIL;


	//if (FAILED(m_pGameInstance->Ready_RT_Debug(TEXT("Target_Result"), 100.f, 900.f - 100.f, 200.f, 200.f)))
	//	return E_FAIL;
	//
	//if (FAILED(m_pGameInstance->Ready_RT_Debug(TEXT("Target_Blur_X"), 300.f, 900.f - 100.f,200.f ,200.f)))
	//	return E_FAIL;
	//
	//if (FAILED(m_pGameInstance->Ready_RT_Debug(TEXT("Target_Blur_Y"), 700.f, 900.f - 100.f ,200.f, 200.f)))
	//	return E_FAIL;

#endif

	return S_OK;
}

HRESULT CRenderer::Add_RenderObject(RENDERGROUP eRenderGroup, CGameObject* pRenderObject)
{
	if (eRenderGroup >= RG_END)
		return E_FAIL;

	m_RenderObjects[eRenderGroup].emplace_back(pRenderObject);

	Safe_AddRef(pRenderObject);

	return S_OK;
}

HRESULT CRenderer::Add_DebugComponent(CComponent* pDebugComponent)
{
	m_DebugComponent.emplace_back(pDebugComponent);

	Safe_AddRef(pDebugComponent);

	return S_OK;
}

HRESULT CRenderer::Clear_DebugComponent()
{
	for (auto& iter : m_DebugComponent)
		Safe_Release(iter);

	m_DebugComponent.clear();

	return S_OK;
}

HRESULT CRenderer::Draw()
{

	if (FAILED(Render_Priority()))
		return E_FAIL;

	if (FAILED(Render_NonBlend()))
		return E_FAIL;

	if (FAILED(Render_Lights()))
		return E_FAIL;

	if (FAILED(Render_NonLight()))
		return E_FAIL;

	if (FAILED(Render_Deferred()))
		return E_FAIL;

	//if (FAILED(Render_Blur()))
	//	return E_FAIL;
	//
	//if (FAILED(Render_FinalBlend()))
	//	return E_FAIL;

	if (FAILED(Render_Blend()))
		return E_FAIL;

	if (FAILED(Render_UI()))
		return E_FAIL;

	if (FAILED(Render_Debug()))
		return E_FAIL;


#ifdef _DEBUG
	if (FAILED(Render_Debug()))
		return E_FAIL;
#endif


	return S_OK;
}

HRESULT CRenderer::Render_Priority()
{
	//m_pGameInstance->Begin_MRT(TEXT("MRT_Result"));

	for (auto& pRenderObject : m_RenderObjects[RG_PRIORITY])
	{
		if (nullptr != pRenderObject)
			pRenderObject->Render();

		Safe_Release(pRenderObject);
	}

	m_RenderObjects[RG_PRIORITY].clear();

	//m_pGameInstance->End_MRT();

	return S_OK;
}

HRESULT CRenderer::Render_ShadowObj()
{
	/* Target_LightDepth */
	if (FAILED(m_pGameInstance->Begin_MRT(TEXT("MRT_ShadowObjects"), m_pShadowDSV)))
		return E_FAIL;

	D3D11_VIEWPORT			ViewPortDesc;
	ZeroMemory(&ViewPortDesc, sizeof(D3D11_VIEWPORT));
	ViewPortDesc.TopLeftX = 0;
	ViewPortDesc.TopLeftY = 0;
	ViewPortDesc.Width = (_float)g_iSizeX;
	ViewPortDesc.Height = (_float)g_iSizeY;
	ViewPortDesc.MinDepth = 0.f;
	ViewPortDesc.MaxDepth = 1.f;

	m_pContext->RSSetViewports(1, &ViewPortDesc);


	for (auto& pRenderObject : m_RenderObjects[RG_SHADOWOBJ])
	{
		if (nullptr != pRenderObject)
			pRenderObject->Render();

		Safe_Release(pRenderObject);
	}

	m_RenderObjects[RG_SHADOWOBJ].clear();

	if (FAILED(m_pGameInstance->End_MRT()))
		return E_FAIL;

	ZeroMemory(&ViewPortDesc, sizeof(D3D11_VIEWPORT));
	ViewPortDesc.TopLeftX = 0;
	ViewPortDesc.TopLeftY = 0;
	ViewPortDesc.Width = (_float)1600.0f;
	ViewPortDesc.Height = (_float)900.0f;
	ViewPortDesc.MinDepth = 0.f;
	ViewPortDesc.MaxDepth = 1.f;

	m_pContext->RSSetViewports(1, &ViewPortDesc);

	return S_OK;
}

HRESULT CRenderer::Render_NonBlend()
{
	/* Diffuse + Normal */
	if (FAILED(m_pGameInstance->Begin_MRT(TEXT("MRT_GameObjects"))))
		return E_FAIL;

	for (auto& pRenderObject : m_RenderObjects[RG_NONBLEND])
	{
		if (nullptr != pRenderObject)
			pRenderObject->Render();

		Safe_Release(pRenderObject);
	}

	m_RenderObjects[RG_NONBLEND].clear();

	if (FAILED(m_pGameInstance->End_MRT()))
		return E_FAIL;

	return S_OK;
}
HRESULT CRenderer::Render_Lights()
{
	/* 빛 연산한 결과를 렌더타겟에 기록해준다. (명암 + ) */
	/* Bind : Shade */
	if (FAILED(m_pGameInstance->Begin_MRT(TEXT("MRT_LightAcc"))))
		return E_FAIL;

	if (FAILED(m_pShader->Bind_Matrix("g_WorldMatrix", &m_WorldMatrix)))
		return E_FAIL;
	if (FAILED(m_pShader->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShader->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;
	if (FAILED(m_pShader->Bind_Matrix("g_ViewMatrixInv", &m_pGameInstance->Get_Transform_Inverse_Float4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;
	if (FAILED(m_pShader->Bind_Matrix("g_ProjMatrixInv", &m_pGameInstance->Get_Transform_Inverse_Float4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Bind_RT_ShaderResource(m_pShader, "g_NormalTexture", TEXT("Target_Normal"))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Bind_RT_ShaderResource(m_pShader, "g_DepthTexture", TEXT("Target_Depth"))))
		return E_FAIL;

	if (FAILED(m_pShader->Bind_RawValue("g_vCamPosition", &m_pGameInstance->Get_CamPosition_Float4(), sizeof(_float4))))
		return E_FAIL;

	m_pVIBuffer->Bind_Buffers();

	m_pGameInstance->Render_Lights(m_pShader, m_pVIBuffer);

	if (FAILED(m_pGameInstance->End_MRT()))
		return E_FAIL;


	return S_OK;
}
HRESULT CRenderer::Render_Deferred()
{
	//m_pGameInstance->Begin_MRT(TEXT("MRT_Result"),false);

	if (FAILED(m_pShader->Bind_Matrix("g_WorldMatrix", &m_WorldMatrix)))
		return E_FAIL;
	if (FAILED(m_pShader->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShader->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Bind_RT_ShaderResource(m_pShader, "g_DiffuseTexture", TEXT("Target_Diffuse"))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Bind_RT_ShaderResource(m_pShader, "g_ShadeTexture", TEXT("Target_Shade"))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Bind_RT_ShaderResource(m_pShader, "g_SpecularTexture", TEXT("Target_Specular"))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Bind_RT_ShaderResource(m_pShader, "g_LightDepthTexture", TEXT("Target_LightDepth"))))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Bind_RT_ShaderResource(m_pShader, "g_DepthTexture", TEXT("Target_Depth"))))
		return E_FAIL;

	m_pShader->Begin(3);
	m_pVIBuffer->Bind_Buffers();
	m_pVIBuffer->Render();

	//m_pGameInstance->End_MRT();

	return S_OK;
}
HRESULT CRenderer::Render_NonLight()
{
	for (auto& pRenderObject : m_RenderObjects[RG_NONLIGHT])
	{
		if (nullptr != pRenderObject)
			pRenderObject->Render();

		Safe_Release(pRenderObject);
	}

	m_RenderObjects[RG_NONLIGHT].clear();

	return S_OK;
}

HRESULT CRenderer::Render_Blend()
{
	for (auto& pRenderObject : m_RenderObjects[RG_BLEND])
	{
		if (nullptr != pRenderObject)
			pRenderObject->Render();

		Safe_Release(pRenderObject);
	}

	m_RenderObjects[RG_BLEND].clear();

	return S_OK;
}

HRESULT CRenderer::Render_UI()
{
	for (auto& pRenderObject : m_RenderObjects[RG_UI])
	{
		if (nullptr != pRenderObject)
			pRenderObject->Render();

		Safe_Release(pRenderObject);
	}

	m_RenderObjects[RG_UI].clear();

	return S_OK;
}

HRESULT CRenderer::Render_Blur()
{
	//if (FAILED(m_pGameInstance->Begin_MRT(TEXT("MRT_Blur_X"))))
	//	return E_FAIL;

	if (FAILED(m_pShader->Bind_Matrix("g_WorldMatrix", &m_WorldMatrix)))
		return E_FAIL;
	if (FAILED(m_pShader->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShader->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	/* g_ResultTexture라는 쉐이더 전역변수에다가 최종적인 장면의 텍스쳐를 전달해준다. */
	//if (FAILED(m_pGameInstance->Bind_RT_ShaderResource(m_pShader,"g_ResultTexture",TEXT("Target_Result"))))
	//	return E_FAIL;
	
	m_pVIBuffer->Bind_Buffers();

	m_pShader->Begin(4);

	m_pVIBuffer->Render();

	if (FAILED(m_pGameInstance->End_MRT()))
		return E_FAIL;

	//if (FAILED(m_pGameInstance->Begin_MRT(TEXT("MRT_Blur_Y"))))
	//	return E_FAIL;

	//if (FAILED(m_pGameInstance->Bind_RT_ShaderResource(m_pShader ,  "g_ResultTexture", TEXT("Target_Blur_X"))))
	//	return E_FAIL;

	m_pVIBuffer->Bind_Buffers();

	m_pShader->Begin(5);

	m_pVIBuffer->Render();

	if (FAILED(m_pGameInstance->End_MRT()))
		return E_FAIL;

	return S_OK;
}

HRESULT CRenderer::Render_FinalBlend()
{
	if (FAILED(m_pShader->Bind_Matrix("g_WorldMatrix", &m_WorldMatrix)))
		return E_FAIL;
	if (FAILED(m_pShader->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShader->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;


	/* 가로로 블러먹이고 세로로 블러먹인 장면을 g_BlurTexture에다가 전ㄷ라한다.ㅏ */
	//if (FAILED(m_pGameInstance->Bind_RT_ShaderResource( m_pShader, "g_BlurTexture", TEXT("Target_Blur_Y"))))
	//	return E_FAIL;

	//if (FAILED(m_pGameInstance->Bind_RenderTarget_ShaderResource(TEXT("Target_Effect"), m_pShader, "g_EffectTexture")))
	//	return E_FAIL;

	/* 최종적인 장면을 g_ResultTexture에다가 전달한다. 블러가 안먹은 장면 */
	//if (FAILED(m_pGameInstance->Bind_RT_ShaderResource(m_pShader, "g_ResultTexture", TEXT("Target_Result"))))
	//	return E_FAIL;

	m_pVIBuffer->Bind_Buffers();

	m_pShader->Begin(6);

	m_pVIBuffer->Render();


	return S_OK;
}

#ifdef _DEBUG

HRESULT CRenderer::Render_Debug()
{
	for (auto& pComponent : m_DebugComponent)
	{
		pComponent->Render();

		Safe_Release(pComponent);
	}

	m_DebugComponent.clear();

	if (FAILED(m_pShader->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShader->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Render_RT_Debug(TEXT("MRT_GameObjects"), m_pShader, m_pVIBuffer)))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Render_RT_Debug(TEXT("MRT_LightAcc"), m_pShader, m_pVIBuffer)))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Render_RT_Debug(TEXT("MRT_ShadowObjects"), m_pShader, m_pVIBuffer)))
		return E_FAIL;

	//if(FAILED(m_pGameInstance->Render_RT_Debug(TEXT("MRT_Result"), m_pShader, m_pVIBuffer)))
	//	return E_FAIL;
	//
	//if (FAILED(m_pGameInstance->Render_RT_Debug(TEXT("MRT_Blur_X"), m_pShader, m_pVIBuffer)))
	//	return E_FAIL;
	//
	//if(FAILED(m_pGameInstance->Render_RT_Debug(TEXT("MRT_Blur_Y"), m_pShader, m_pVIBuffer)))
	//	return E_FAIL;
	//if (FAILED(m_pGameInstance->Render_RT_Debug(TEXT("MRT_Sky"), m_pShader, m_pVIBuffer)))
	//	return E_FAIL;
	
	return S_OK;
}

#endif

CRenderer* CRenderer::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CRenderer* pInstance = new CRenderer(pDevice, pContext);

	if (FAILED(pInstance->Initialize(pDevice, pContext)))
	{
		MSG_BOX(TEXT("Failed to Created : CRenderer"));
		Safe_Release(pInstance);
	}

	return pInstance;
}


void CRenderer::Free()
{
	__super::Free();

	for (size_t i = 0; i < RG_END; i++)
	{
		for (auto& pRenderObject : m_RenderObjects[i])
			Safe_Release(pRenderObject);
		m_RenderObjects[i].clear();
	}
	
	Safe_Release(m_pShadowDSV);

	Safe_Release(m_pShader);
	Safe_Release(m_pVIBuffer);

	Safe_Release(m_pGameInstance);
	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
}
