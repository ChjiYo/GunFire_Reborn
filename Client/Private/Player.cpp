#include "stdafx.h"

#include "Player.h"
#include "GameInstance.h"
#include "Camera.h"

#include "FSM_Player_Attack.h"
#include "FSM_Player_Dash.h"

#include "Weapon_Player.h"
#include "Weapon_Player_Pistol.h"

CPlayer::CPlayer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CContainerObject{ pDevice  , pContext }
{
}

CPlayer::CPlayer(const CPlayer& Prototype)
    :CContainerObject{ Prototype }
{
}

HRESULT CPlayer::Initialize_Prototype()
{
    if (FAILED(__super::Initialize_Prototype()))
        return E_FAIL;
    
    return S_OK;
}

HRESULT CPlayer::Initialize(void* pArg)
{
    CContainerObject::CONTAINEROBJECT_DESC		ContainerDesc{};
    ContainerDesc.fRotationPerSec = XMConvertToRadians(90.0f);
    ContainerDesc.fSpeedPerSec = 5.f;
    ContainerDesc.iNumParts = PART_END;

    if (FAILED(__super::Initialize(&ContainerDesc)))
        return E_FAIL;

    PLAYER_DESC* Desc = static_cast<PLAYER_DESC*>(pArg);

    Player_Desc.iHp = Desc->iHp;
    Player_Desc.iMaxHp = Desc->iMaxHp;
    Player_Desc.iShield = Desc->iShield;
    Player_Desc.iMaxShield = Desc->iMaxShield;


    if (FAILED(Ready_PartObjects()))
        return E_FAIL;

    if (FAILED(Ready_Components()))
        return E_FAIL;

    if (FAILED(Ready_State()))
        return E_FAIL;

    m_eWeaponID = PISTOL;

    m_pTransformCom->Set_State(CTransform::STATE_POSITION, { 10.f , 1.3f , 10.f, 1.f });
    m_pNavigationCom->Init_Index(m_pTransformCom);
    
    return S_OK;
}

void CPlayer::Priority_Update(_float fTimeDelta)
{
    if (fTimeDelta == 0.f)
        return;

    if (Player_Desc.iMaxShield > Player_Desc.iShield)
        m_fShieldRecoveryTimer += fTimeDelta;
    else
        m_fShieldRecoveryTimer = 0.f;
    
    Player_Weapon_Change();

    __super::Priority_Update(fTimeDelta);
}

void CPlayer::Update(_float fTimeDelta)
{
    m_pColliderCom->Update(m_pTransformCom->Get_WorldMatrix());

    ShieldRecovery();
        
    __super::Update(fTimeDelta);
}

void CPlayer::Late_Update(_float fTimeDelta)
{
    __super::Late_Update(fTimeDelta);

    Key_State(fTimeDelta);
    m_pGameInstance->Add_RenderObject(CRenderer::RG_NONBLEND, this);
}

HRESULT CPlayer::Render()
{
#ifdef _DEBUG
    m_pColliderCom->Render();
    m_pNavigationCom->Render();
#endif

    return S_OK;
}

void CPlayer::ShieldRecovery()
{
    if (m_fShieldRecoveryTimer > 3.f)
    {
        Player_Desc.iShield++;
    }
}

HRESULT CPlayer::Ready_PartObjects()
{
    m_eWeaponID = PISTOL;
    m_bWeaponChange = true;

    return S_OK;
}

HRESULT CPlayer::Ready_Components()
{
    CNavigation::NAVIGATION_DESC		NavigationDesc{};
    NavigationDesc.iCurrentCellIndex = 0;
    
    /* Com_Navigation */
    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Navigation"),
        TEXT("Com_Navigation"), reinterpret_cast<CComponent**>(&m_pNavigationCom), &NavigationDesc)))
        return E_FAIL;

    /* For.Com_State */
    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_State"),
        TEXT("Com_State"), reinterpret_cast<CComponent**>(&m_pState))))
        return E_FAIL;

    /* For.Com_Collider */
   CBounding_AABB::BOUNDING_AABB_DESC	BoundingDesc{};
   
   BoundingDesc.vExtents = _float3(0.5f, 0.5f, 0.5f);
   BoundingDesc.vCenter = _float3(0.f, BoundingDesc.vExtents.y / 2.f, 0.f);  
   if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_AABB"),
       TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pColliderCom), &BoundingDesc)))
       return E_FAIL;

    return S_OK;
}

HRESULT CPlayer::Ready_State()
{
    m_pState->Add_State(CFSM_Player_Attack::Create(m_pDevice ,m_pContext), TEXT("Player_State_Attack"));
    m_pState->Add_State(CFSM_Player_Dash::Create(m_pDevice, m_pContext), TEXT("Player_State_Dash"));

    return S_OK;
}
void CPlayer::Key_State(_float fTimeDelta)
{
    Player_Move(fTimeDelta);
    Player_Jump(fTimeDelta);
}

void CPlayer::Player_Move(_float fTimeDelta)
{
    _long		MouseMove = {};

    if (MouseMove = m_pGameInstance->Get_DIMouseMove(DIMM_X))
    {
        m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), 0.1f * MouseMove * fTimeDelta);
    }

    if (MouseMove = m_pGameInstance->Get_DIMouseMove(DIMM_Y))
    {
        m_pTransformCom->Turn(m_pTransformCom->Get_State(CTransform::STATE_RIGHT), 0.1f * MouseMove * fTimeDelta);
    }

    if (m_pGameInstance->Get_DIKeyState(DIK_A) & 0x80)
    {
        m_pTransformCom->Go_Left(fTimeDelta, m_pNavigationCom);

        if (m_pGameInstance->Key_Down(VK_LSHIFT))
            m_pTransformCom->Go_Left(fTimeDelta + 0.5f, m_pNavigationCom);
    }

    if (m_pGameInstance->Get_DIKeyState(DIK_D) & 0x80)
    {
        m_pTransformCom->Go_Right(fTimeDelta, m_pNavigationCom);

        if (m_pGameInstance->Key_Down(VK_LSHIFT))
            m_pTransformCom->Go_Right(fTimeDelta + 0.5f, m_pNavigationCom);
    }

    if (m_pGameInstance->Get_DIKeyState(DIK_W) & 0x80)
    {
        m_pTransformCom->Go_StraightY(fTimeDelta, m_pNavigationCom);

        if (m_pGameInstance->Key_Down(VK_LSHIFT))
            m_pTransformCom->Go_StraightY(fTimeDelta + 0.5f, m_pNavigationCom);
        
    }

    if (m_pGameInstance->Get_DIKeyState(DIK_S) & 0x80)
    {
        m_pTransformCom->Go_BackwardY(fTimeDelta, m_pNavigationCom);

        if (m_pGameInstance->Key_Down(VK_LSHIFT))
            m_pTransformCom->Go_BackwardY(fTimeDelta + 0.5f, m_pNavigationCom);
    }

    if (m_pGameInstance->Key_Down('1'))
    {
        m_eWeaponID = SHURIKEN;
        m_bWeaponChange = true;
    }

    if (m_pGameInstance->Key_Down('2'))
    {
        m_bWeaponChange = true;
    }

    if (m_pGameInstance->Key_Down('3'))
    {
        m_eWeaponID = PISTOL;
        m_bWeaponChange = true;
    }
}

void CPlayer::Player_Jump(_float fTimeDelta)
{
    if (m_pGameInstance->Key_Down(VK_SPACE)|| m_bJump)
    {
        m_pTransformCom->Go_Up(fTimeDelta);
        m_bJump = true;
    }

    if (m_pTransformCom->Gravity(0.2f, 1.3f, fTimeDelta) && m_bJump)
    {
        m_bJump = false;
    }
}

void CPlayer::Player_Weapon_Change()
{
    if (m_bWeaponChange)
    {
        Safe_Release(__super::Get_Parts().front());
        __super::Get_Parts().clear();

        CWeapon_Player::PLAYER_WEAPON_DESC Desc{};
        Desc.pParentMatrix = m_pTransformCom->Get_WorldMatrixPtr();
        Desc.pParentState = 0x00000000;
        

        switch (m_eWeaponID)
        {
        case PISTOL:
            Desc.iNumBullet = 30;
            Desc.fAttackSpeed = 0.25f;
            __super::Add_PartObject(PART_WEAPON, TEXT("Prototype_GameObject_Weapon_Player_Pistol"), &Desc);
            break;

        case SHURIKEN:
            __super::Add_PartObject(PART_WEAPON, TEXT("Prototype_GameObject_Weapon_Player_Shuriken"), &Desc);
            break;
        }

        m_bWeaponChange = false;
    }
}

CPlayer* CPlayer::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CPlayer* pInstance = new CPlayer(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX(TEXT("Failed to Created : CPlayer"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CPlayer::Clone(void* pArg)
{
    CPlayer* pInstance = new CPlayer(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX(TEXT("Failed to Cloned : CPlayer"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CPlayer::Free()
{
    Safe_Release(m_pState);
    Safe_Release(m_pColliderCom);
    Safe_Release(m_pNavigationCom);

    __super::Free();
}
