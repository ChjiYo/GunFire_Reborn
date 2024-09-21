#include "stdafx.h"

#include "Weapon_Player_Sniper.h"
#include "GameInstance.h"
#include "Camera_Free.h"

#include "FSM_Player_Attack.h"
#include "FSM_Player_Dash.h"

#include "Skill.h"
#include "VFX.h"

#include "UI_Sniper_Hair.h"
#include "UI_Sniper_Ring.h"

CWeapon_Player_Sniper::CWeapon_Player_Sniper(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CWeapon_Player{ pDevice  , pContext }
{
}

CWeapon_Player_Sniper::CWeapon_Player_Sniper(const CWeapon_Player_Sniper& Prototype)
    :CWeapon_Player{ Prototype }
{
}

HRESULT CWeapon_Player_Sniper::Initialize_Prototype()
{
    if (FAILED(__super::Initialize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CWeapon_Player_Sniper::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(Ready_Components()))
        return E_FAIL;

    PLAYER_WEAPON_DESC* Desc = static_cast<PLAYER_WEAPON_DESC*>(pArg);
    m_iNumMaxBullet = Desc->iMaxBullet;
    m_iNumBullet = Desc->iNumBullet;
    m_fAttackSpeed = Desc->fAttackSpeed;
    m_pPlayerTransform = Desc->pTargetTransform;
    Safe_AddRef(m_pPlayerTransform);

    m_iOriginBullet = 8;

    m_pTransformCom->Set_State(CTransform::STATE_POSITION, { 0.25f , -0.15f , -0.15f , 1.f });

    m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_UI_Sniper_Hair"), TEXT("Layer_UI_Sniper"));
    

    return S_OK;
}

void CWeapon_Player_Sniper::Priority_Update(_float fTimeDelta)
{
    if (fTimeDelta == 0.f)
        return;

    m_fAttackTimer += fTimeDelta;
    m_fReboundTimer += fTimeDelta;

    Key_State(fTimeDelta);

    

    m_pModelCom->Play_Animation(fTimeDelta);

    __super::Priority_Update(fTimeDelta);
}

void CWeapon_Player_Sniper::Update(_float fTimeDelta)
{
    __super::Update(fTimeDelta);
}

void CWeapon_Player_Sniper::Late_Update(_float fTimeDelta)
{
    __super::Late_Update(fTimeDelta);

    m_pGameInstance->Add_RenderObject(CRenderer::RG_NONBLEND, this);
}

HRESULT CWeapon_Player_Sniper::Render()
{
    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;

    _uint	iNumMeshes = m_pModelCom->Get_NumMeshes();

    if (FAILED(m_pModelCom->Bind_MaterialSRV(m_pShaderCom, aiTextureType_DIFFUSE, "g_DiffuseTexture", 0)))
        return E_FAIL;

    for (size_t i = 0; i < iNumMeshes; i++)
    {
        if (FAILED(m_pModelCom->Bind_BoneMatrices(m_pShaderCom, "g_BoneMatrices", (_uint)i)))
            return E_FAIL;

        if (FAILED(m_pShaderCom->Begin(0)))
            return E_FAIL;

        if (FAILED(m_pModelCom->Render(i)))
            return E_FAIL;
    }

    return S_OK;
}

HRESULT CWeapon_Player_Sniper::Ready_Components()
{
    /* For.Com_Shader */
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxAnimMesh"),
        TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
        return E_FAIL;

    /* For.Com_Model */
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Model_Weapon_Player_Sniper"),
        TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
        return E_FAIL;

    return S_OK;
}

HRESULT CWeapon_Player_Sniper::Bind_ShaderResources()
{
    if (FAILED(__super::Bind_WorldMatrix(m_pShaderCom, "g_WorldMatrix")))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
        return E_FAIL;

    return S_OK;
}

void CWeapon_Player_Sniper::Key_State(_float fTimeDelta)
{
    if (m_pGameInstance->Key_Down(VK_LBUTTON) && m_iNumBullet > 0 && m_fAttackTimer > m_fAttackSpeed)
    {
        m_iNumBullet--;
        m_pModelCom->SetUp_Animation(0, false);
        Create_Bullet();
        m_fAttackTimer = 0.f;

        m_bReboundEnble = true;

    }
    if(m_bReboundEnble)
        Rebound();


    if (m_pGameInstance->Key_Down('R'))
    {
        if (m_iNumMaxBullet <= 0)
            return;

        _uint iNumReload = m_iOriginBullet - m_iNumBullet;

        m_iNumMaxBullet >= iNumReload ? (m_iNumBullet += iNumReload, m_iNumMaxBullet -= iNumReload) : (m_iNumBullet += m_iNumMaxBullet, m_iNumMaxBullet = 0);

        m_pModelCom->SetUp_Animation(2, false);
    }
    
    _float fLastTime = 0.25f;
    if (m_fZoomTimer <= fLastTime && (m_pGameInstance->Key_Down(VK_RBUTTON) || m_bZoomCheck) )
    {
        m_fZoomTimer += fTimeDelta;

        if (!m_bZoomCheck)
        {
            m_pModelCom->SetUp_Animation(4, false);

            
            CUI_Sniper_Hair::SNIPER_HAIR_DESC SniperDesc = {};

            for (int i = 0; i < 4; ++i)
            {
                SniperDesc.iNum = i;
                m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_SniperCrossHair"), TEXT("Layer_UI_Sniper"),&SniperDesc);
            }
            m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_UI_SniperRing"), TEXT("Layer_UI_Sniper"), &SniperDesc);
        }

        m_bZoomCheck = true;

        _vector vOriginPos = { 0.25f , -0.15f , -0.15f , 1.f };
        _vector vLastPos = { 0.125f ,-0.2f ,-0.35f , 1.f };
        _vector vDistance = vLastPos - vOriginPos;

        _vector vZoomSpeed = vDistance / fLastTime;

        _vector vPos = { XMVectorGetX(vZoomSpeed) * m_fZoomTimer,XMVectorGetY(vZoomSpeed) * m_fZoomTimer,XMVectorGetZ(vZoomSpeed) * m_fZoomTimer, 1.f };

        m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);

    }

    if (m_fZoomTimer > fLastTime)
    {
        CCamera_Free* pCamera = dynamic_cast<CCamera_Free*>(m_pGameInstance->Get_Layer(m_pGameInstance->GetLoadingLevelIndex(), TEXT("Layer_Camera")).back());
        pCamera->Set_CameraType(CCamera_Free::SCOPE);

        if (m_pGameInstance->Key_Down(VK_RBUTTON))
        {
            pCamera->Set_CameraType(CCamera_Free::PLAYER);
            m_bZoomCheck = false;
            m_fZoomTimer = 0.f;

            m_pTransformCom->Set_State(CTransform::STATE_POSITION, { 0.25f , -0.15f , -0.15f , 1.f });

            for (auto& iter : m_pGameInstance->Get_Layer(LEVEL_GAMEPLAY, TEXT("Layer_UI_Sniper")))
                iter->Set_Dead();
            
        }
    }
  
}

void CWeapon_Player_Sniper::Create_Bullet()
{
    CSkill::SKILL_DESC	Desc = {};

    Desc.pOriginTransform = dynamic_cast<CTransform*>(m_pGameInstance->Get_Component(m_pGameInstance->GetLoadingLevelIndex(), TEXT("Layer_Player"), TEXT("Com_Transform")));
    Desc.fSpeedPerSec = 5.f;
    Desc.fRotationPerSec = XMConvertToRadians(90.0f);
    Desc.iSkillDamage = m_iBulletDamage;

    m_pGameInstance->Add_GameObject_ToLayer(m_pGameInstance->GetCurrentLevelIndex(), TEXT("Prototype_GameObject_Skill_Sniper_Bullet"), TEXT("Layer_Skill_Player_Bullet"), &Desc);

    CVFX::VFX_DESC VFX_Desc = {};
    VFX_Desc.pOriginTransform = m_pPlayerTransform;

    _bool		isPick = { false };

    VFX_Desc.vCreatePos = XMLoadFloat4(&m_pGameInstance->Picked_Position(&isPick));
    m_pGameInstance->Add_GameObject_ToLayer(m_pGameInstance->GetCurrentLevelIndex(), TEXT("Prototype_GameObject_VFX_Sniper"), TEXT("Layer_Effect"), &VFX_Desc);
}

void CWeapon_Player_Sniper::Rebound()
{
    CTransform* pPlayerTransform = dynamic_cast<CTransform*>(m_pGameInstance->Get_Component(m_pGameInstance->GetCurrentLevelIndex(), TEXT("Layer_Player"), TEXT("Com_Transform")));

    if (m_fReboundTimer >= 0.01f)
    {
        m_fReboundAngle -= 2.f;
        m_fReboundTimer = 0.f;
        pPlayerTransform->Radian_Turn(pPlayerTransform->Get_State(CTransform::STATE_RIGHT), XMConvertToRadians(m_fReboundAngle));
    }

    if (m_fReboundAngle <= -6.f)
    {
        m_bReboundEnble = false;
        m_fReboundTimer = 0.f;
        m_fReboundAngle = 0.f;
    }
 
}

void CWeapon_Player_Sniper::Zoom()
{
    
}

CWeapon_Player_Sniper* CWeapon_Player_Sniper::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CWeapon_Player_Sniper* pInstance = new CWeapon_Player_Sniper(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX(TEXT("Failed to Created : CWeapon_Player_Sniper"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CWeapon_Player_Sniper::Clone(void* pArg)
{
    CWeapon_Player_Sniper* pInstance = new CWeapon_Player_Sniper(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX(TEXT("Failed to Cloned : CWeapon_Player_Sniper"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CWeapon_Player_Sniper::Free()
{
    Safe_Release(m_pShaderCom);
    Safe_Release(m_pModelCom);
    Safe_Release(m_pPlayerTransform);

    __super::Free();
}
