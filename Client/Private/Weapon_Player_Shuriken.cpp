#include "stdafx.h"

#include "Weapon_Player_Shuriken.h"
#include "GameInstance.h"
#include "Camera.h"

#include "FSM_Player_Attack.h"
#include "FSM_Player_Dash.h"

#include "Skill.h"

CWeapon_Player_Shuriken::CWeapon_Player_Shuriken(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CWeapon_Player{ pDevice  , pContext }
{
}

CWeapon_Player_Shuriken::CWeapon_Player_Shuriken(const CWeapon_Player_Shuriken& Prototype)
    :CWeapon_Player{ Prototype }
{
}

HRESULT CWeapon_Player_Shuriken::Initialize_Prototype()
{
    if (FAILED(__super::Initialize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CWeapon_Player_Shuriken::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(Ready_Components()))
        return E_FAIL;

    return S_OK;
}

void CWeapon_Player_Shuriken::Priority_Update(_float fTimeDelta)
{
    if (fTimeDelta == 0.f)
        return;

    Key_State();

    m_pModelCom->Play_Animation(fTimeDelta);

    __super::Priority_Update(fTimeDelta);
}

void CWeapon_Player_Shuriken::Update(_float fTimeDelta)
{
    __super::Update(fTimeDelta);
}

void CWeapon_Player_Shuriken::Late_Update(_float fTimeDelta)
{
    __super::Late_Update(fTimeDelta);

    m_pGameInstance->Add_RenderObject(CRenderer::RG_NONBLEND, this);
}

HRESULT CWeapon_Player_Shuriken::Render()
{
    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;

    _uint	iNumMeshes = m_pModelCom->Get_NumMeshes();

    for (size_t i = 0; i < iNumMeshes; i++)
    {
        if (FAILED(m_pModelCom->Bind_MaterialSRV(m_pShaderCom, aiTextureType_DIFFUSE, "g_DiffuseTexture", i)))
            return E_FAIL;

        if (FAILED(m_pModelCom->Bind_BoneMatrices(m_pShaderCom, "g_BoneMatrices", (_uint)i)))
            return E_FAIL;

        if (FAILED(m_pShaderCom->Begin(0)))
            return E_FAIL;

        if (FAILED(m_pModelCom->Render(i)))
            return E_FAIL;
    }

    return S_OK;
}

HRESULT CWeapon_Player_Shuriken::Ready_Components()
{
    /* For.Com_Shader */
    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxAnimMesh"),
        TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
        return E_FAIL;

    /* For.Com_Model */
    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Weapon_Player_Shuriken"),
        TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
        return E_FAIL;

    return S_OK;
}

HRESULT CWeapon_Player_Shuriken::Bind_ShaderResources()
{
    if (FAILED(__super::Bind_WorldMatrix(m_pShaderCom, "g_WorldMatrix")))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
        return E_FAIL;

    return S_OK;
}

void CWeapon_Player_Shuriken::Key_State()
{
    if (m_pGameInstance->Key_Down(VK_LBUTTON))
    {
        m_pModelCom->SetUp_Animation(0, false);
        Create_Bullet();
    }
}

void CWeapon_Player_Shuriken::Create_Bullet()
{
    CSkill::SKILL_DESC	Desc = {};

    Desc.pOriginTransform = dynamic_cast<CTransform*>(m_pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_Player"), TEXT("Com_Transform")));
    Desc.fSpeedPerSec = 5.f;
    Desc.fRotationPerSec = XMConvertToRadians(90.0f);

    m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Skill_Shuriken_Bullet"), TEXT("Layer_Skill_Player_Bullet"), &Desc);
}

CWeapon_Player_Shuriken* CWeapon_Player_Shuriken::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CWeapon_Player_Shuriken* pInstance = new CWeapon_Player_Shuriken(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX(TEXT("Failed to Created : CWeapon_Player_Shuriken"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CWeapon_Player_Shuriken::Clone(void* pArg)
{
    CWeapon_Player_Shuriken* pInstance = new CWeapon_Player_Shuriken(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX(TEXT("Failed to Cloned : CWeapon_Player_Shuriken"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CWeapon_Player_Shuriken::Free()
{
    Safe_Release(m_pShaderCom);
    Safe_Release(m_pModelCom);

    __super::Free();
}
