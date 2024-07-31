#include "stdafx.h"

#include "Weapon_Player_Pistol.h"
#include "GameInstance.h"
#include "Camera.h"

#include "FSM_Player_Attack.h"
#include "FSM_Player_Dash.h"

#include "Skill.h"

CWeapon_Player_Pistol::CWeapon_Player_Pistol(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CWeapon_Player{ pDevice  , pContext }
{
}

CWeapon_Player_Pistol::CWeapon_Player_Pistol(const CWeapon_Player_Pistol& Prototype)
    :CWeapon_Player{ Prototype }
{
}

HRESULT CWeapon_Player_Pistol::Initialize_Prototype()
{
    if (FAILED(__super::Initialize_Prototype()))
        return E_FAIL;

    return S_OK; 
}

HRESULT CWeapon_Player_Pistol::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    PLAYER_WEAPON_DESC* Desc = static_cast<PLAYER_WEAPON_DESC*>(pArg);
    m_iNumMaxBullet = Desc->iNumBullet;
    m_iNumBullet = m_iNumMaxBullet;
    m_fAttackSpeed = Desc->fAttackSpeed;

    if (FAILED(Ready_Components()))
        return E_FAIL;

    return S_OK;
}

void CWeapon_Player_Pistol::Priority_Update(_float fTimeDelta)
{
    if (fTimeDelta == 0.f)
        return;

    m_fAttackTimer += fTimeDelta;

    Key_State();

    m_pModelCom->Play_Animation(fTimeDelta);

    __super::Priority_Update(fTimeDelta);
}

void CWeapon_Player_Pistol::Update(_float fTimeDelta)
{
    __super::Update(fTimeDelta);
}

void CWeapon_Player_Pistol::Late_Update(_float fTimeDelta)
{
    __super::Late_Update(fTimeDelta);

    m_pGameInstance->Add_RenderObject(CRenderer::RG_NONBLEND, this);
}

HRESULT CWeapon_Player_Pistol::Render()
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
    wstring strNumBullet = to_wstring(m_iNumBullet);
    wstring strNumMaxBullet = to_wstring(m_iNumMaxBullet);

    m_pGameInstance->Draw_Font(TEXT("Font_Nexon"), strNumBullet, _float2(1300.f ,  800.f), XMVectorSet(0.7f, 1.f, 0.3f, 1.f), XMConvertToRadians(-5.f), 0.8f);
    m_pGameInstance->Draw_Font(TEXT("Font_Nexon"), strNumMaxBullet, _float2(1450.f, 800.f), XMVectorSet(0.7f, 1.f, 0.3f, 1.f), XMConvertToRadians(-5.f), 0.8f);


    return S_OK;
}

HRESULT CWeapon_Player_Pistol::Ready_Components()
{
    /* For.Com_Shader */
    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxAnimMesh"),
        TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
        return E_FAIL;

    /* For.Com_Model */
    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Weapon_Player_Pistol"),
        TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
        return E_FAIL;    

    return S_OK;
}

HRESULT CWeapon_Player_Pistol::Bind_ShaderResources()
{
    if (FAILED(__super::Bind_WorldMatrix(m_pShaderCom, "g_WorldMatrix")))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_RawValue("g_vCamPosition", &m_pGameInstance->Get_CamPosition_Float4(), sizeof(_float4))))
        return E_FAIL;

    const LIGHT_DESC* pLightDesc = m_pGameInstance->Get_LightDesc(0);
    if (nullptr == pLightDesc)
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_RawValue("g_vLightDir", &pLightDesc->vDirection, sizeof(_float4))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_vLightDiffuse", &pLightDesc->vDiffuse, sizeof(_float4))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_vLightAmbient", &pLightDesc->vDiffuse, sizeof(_float4))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_RawValue("g_vLightSpecular", &pLightDesc->vSpecular, sizeof(_float4))))
        return E_FAIL;

    return S_OK;
}

void CWeapon_Player_Pistol::Key_State()
{   
    if (m_pGameInstance->Key_Down(VK_LBUTTON) && m_iNumBullet > 0  && m_fAttackTimer > m_fAttackSpeed)
    {
        m_iNumBullet--;
        m_pModelCom->SetUp_Animation(1, false);
        Create_Bullet();
        m_fAttackTimer = 0.f;
    }

    if (m_pGameInstance->Key_Down('R'))
    {
        m_iNumBullet = m_iNumMaxBullet;
    }
}

void CWeapon_Player_Pistol::Create_Bullet()
{
    CSkill::SKILL_DESC	Desc = {};

	Desc.pTargetTransform = dynamic_cast<CTransform*>(m_pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_Player"), TEXT("Com_Transform")));
	Desc.fSpeedPerSec = 5.f;
	Desc.fRotationPerSec = XMConvertToRadians(90.0f);
    Desc.iTextureIndex = 0;

	m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Skill_Bullet_Red"),TEXT("Layer_Skill_Player_Bullet"), &Desc);
}


CWeapon_Player_Pistol* CWeapon_Player_Pistol::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CWeapon_Player_Pistol* pInstance = new CWeapon_Player_Pistol(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX(TEXT("Failed to Created : CWeapon_Player_Pistol"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CWeapon_Player_Pistol::Clone(void* pArg)
{
    CWeapon_Player_Pistol* pInstance = new CWeapon_Player_Pistol(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX(TEXT("Failed to Cloned : CWeapon_Player_Pistol"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CWeapon_Player_Pistol::Free()
{
    Safe_Release(m_pShaderCom);
    Safe_Release(m_pModelCom);

    __super::Free();
}
