#include "stdafx.h"

#include "Tool.h"

CTool::CTool(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
     : m_pDevice{ pDevice }
    , m_pContext{ pContext }
    , m_pGameInstance{ CGameInstance::Get_Instance() }
{
    Safe_AddRef(m_pGameInstance);
    Safe_AddRef(m_pContext);
    Safe_AddRef(m_pDevice);
}

HRESULT CTool::Initialize()
{
    return S_OK;
}

HRESULT CTool::Render()
{
    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("Option")) {
            if (ImGui::MenuItem("IMGUI ON", NULL, &m_bShowImGuiWindows)) {
                // bShowImGuiWindows 값이 클릭할 때마다 토글됩니다.
            }
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }

    return S_OK;
}

void CTool::Free()
{
    Safe_Release(m_pGameInstance);
    Safe_Release(m_pContext);
    Safe_Release(m_pDevice);

    __super::Free();
}
