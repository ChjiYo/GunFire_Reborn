#include "stdafx.h"

#include "Tool_Manager.h"

CTool_Manager::CTool_Manager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CTool { pDevice, pContext }
{
}

HRESULT CTool_Manager::Initialize()
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGui::StyleColorsDark();

    ImGui_ImplWin32_Init(g_hWnd);
    ImGui_ImplDX11_Init(m_pDevice, m_pContext);

    if (FAILED(Ready_Init()))
        return E_FAIL;

    return S_OK;
}

HRESULT CTool_Manager::Tool_Render()
{
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    if (FAILED(__super::Render()))
        return E_FAIL;

    if(FAILED(m_pTool_Object->Render()))
        return E_FAIL;

    if (FAILED(m_pTool_Save->Render()))
        return E_FAIL;

    return S_OK;
}

HRESULT CTool_Manager::Tool_RenderEnd()
{
    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

    return S_OK;
}

HRESULT CTool_Manager::Ready_Init()
{
    m_pTool_Object = CTool_Object::Create(m_pDevice, m_pContext);
    if (m_pTool_Object == nullptr)
        return E_FAIL;

    m_pTool_Save = CTool_Save::Create(m_pDevice, m_pContext);
    if (m_pTool_Object == nullptr)
        return E_FAIL;

    return S_OK;
}

CTool_Manager* CTool_Manager::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CTool_Manager* pInstance = new CTool_Manager(pDevice, pContext);

    if (FAILED(pInstance->Initialize()))
    {
        MSG_BOX(TEXT("Failed to Created : CTool_Manager"));
    }

    return pInstance;
}

void CTool_Manager::Free()
{
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    Safe_Release(m_pTool_Object);
    Safe_Release(m_pTool_Save);

    __super::Free();
}
