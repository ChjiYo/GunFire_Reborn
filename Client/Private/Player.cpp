#include "stdafx.h"

#include "Player.h"
#include "GameInstance.h"
#include "Camera.h"

#include "FSM_Player_Attack.h"
#include "FSM_Player_Dash.h"

#include "Weapon_Player.h"
#include "Weapon_Player_Pistol.h"

#include "UI_Weapon_Icon.h"
#include "Monster.h"

#include "Door.h"
#include "Cell.h"

#include "Level_Loading.h"

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

    if (FAILED(Ready_Components()))
        return E_FAIL;

    m_eWeaponID = PISTOL;
    m_eWeaponIDArray[2] = PISTOL;

    switch (m_pGameInstance->GetLoadingLevelIndex())
    {
    case LEVEL_GAMEPLAY:
        m_pTransformCom->Set_State(CTransform::STATE_POSITION, { 47.f , 1.0f , 64.f, 1.f });
        break;

    case LEVEL_BOSS:
        m_pTransformCom->Set_State(CTransform::STATE_POSITION, { 106.f , 1.0f , 70.f, 1.f });
        break;
    }

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

    m_fInvincibleTime += fTimeDelta;
    m_fPosSaveTimer += fTimeDelta;
    m_fMoveSkillTimer += fTimeDelta;

    __super::Priority_Update(fTimeDelta);
}

void CPlayer::Update(_float fTimeDelta)
{
    __super::Update(fTimeDelta);

    m_pColliderCom->Update(m_pTransformCom->Get_WorldMatrix());

    ShieldRecovery();
    Player_Weapon_Change();

    _vector vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

    if (m_pGameInstance->Key_Down('B'))
        Set_Damage(-5);   
}

void CPlayer::Late_Update(_float fTimeDelta)
{
    __super::Late_Update(fTimeDelta);

    Key_State(fTimeDelta);
    PositionSave();

    list<CGameObject*> ObjList = m_pGameInstance->Get_Layer(LEVEL_GAMEPLAY, TEXT("Layer_Monster"));

    for (auto& Monsteriter : ObjList)
    {
        if ((m_pColliderCom->isCollision(dynamic_cast<CMonster*>(Monsteriter)->Get_Collider(CMonster::COLL_AABB))) && m_fInvincibleTime > 1.f)
        {
            Set_Damage(-5.f);
            m_fInvincibleTime = 0.f;
        }
    }

    m_pGameInstance->Add_RenderObject(CRenderer::RG_NONBLEND, this);
}

HRESULT CPlayer::Render() 
{

    return S_OK;
}

void CPlayer::ShieldRecovery()
{
    if (m_fShieldRecoveryTimer > 3.f)
    {
        Player_Desc.iShield++;
    }
}

void CPlayer::PositionSave()
{
    if (m_fPosSaveTimer > 0.01 && !m_bBackPos)
    {
        m_fPosSaveTimer = 0.f;
        m_StackPos.push_back(m_pTransformCom->Get_WorldMatrix());
    }

    if ((m_pGameInstance->Key_Down('I') || m_bBackPos) && m_fMoveSkillTimer > 0.0001f)
    {
        m_bBackPos = true;

        m_pTransformCom->Set_State(CTransform::STATE_RIGHT, m_StackPos.back().r[CTransform::STATE_RIGHT]);
        m_pTransformCom->Set_State(CTransform::STATE_UP, m_StackPos.back().r[CTransform::STATE_UP]);
        m_pTransformCom->Set_State(CTransform::STATE_LOOK, m_StackPos.back().r[CTransform::STATE_LOOK]);
        m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_StackPos.back().r[CTransform::STATE_POSITION]);
        m_StackPos.pop_back();
        m_fMoveSkillTimer = 0.f;


        list<CGameObject*> MonsterList = m_pGameInstance->Get_Layer(m_pGameInstance->GetCurrentLevelIndex(), TEXT("Layer_Monster_Stage2"));

        for (auto& iter : MonsterList)
        {
            dynamic_cast<CMonster*>(iter)->SetMoveEnable(false);
        }
    }

    if (m_StackPos.empty())
    {
        list<CGameObject*> MonsterList = m_pGameInstance->Get_Layer(m_pGameInstance->GetCurrentLevelIndex(), TEXT("Layer_Monster_Stage2"));

        for (auto& iter : MonsterList)
        {
            dynamic_cast<CMonster*>(iter)->SetMoveEnable(true);
        }

        m_bBackPos = false;
        m_pNavigationCom->Init_Index(m_pTransformCom);
    }

    if (m_StackPos.size() >= 200)
    {
        m_StackPos.erase(m_StackPos.begin());
    }
}

HRESULT CPlayer::Ready_Components()
{
    CNavigation::NAVIGATION_DESC		NavigationDesc{};
    NavigationDesc.iCurrentCellIndex = 0;
    
    /* Com_Navigation */
    if (FAILED(__super::Add_Component(m_pGameInstance->GetLoadingLevelIndex(), TEXT("Prototype_Component_Navigation"),
        TEXT("Com_Navigation"), reinterpret_cast<CComponent**>(&m_pNavigationCom), &NavigationDesc)))
        return E_FAIL;

    /* For.Com_Collider */
   CBounding_AABB::BOUNDING_AABB_DESC	BoundingDesc{};

   BoundingDesc.vExtents = _float3(1.0f, 1.0f, 1.0f);
   BoundingDesc.vCenter = _float3(0.f, BoundingDesc.vExtents.y * 0.5f  , 0.f);  
   BoundingDesc.pObject = this;

   if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_AABB"),
       TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pColliderCom), &BoundingDesc)))
       return E_FAIL;

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
    _bool isMove = true;

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
        m_pTransformCom->Go_Left(fTimeDelta, m_pNavigationCom , m_bJump);

        if (m_pGameInstance->Key_Down(VK_LSHIFT))
            m_pTransformCom->Go_Left(fTimeDelta  * 15.5f, m_pNavigationCom);
    }

    if (m_pGameInstance->Get_DIKeyState(DIK_D) & 0x80)
    {
        m_pTransformCom->Go_Right(fTimeDelta, m_pNavigationCom, m_bJump);

        if (m_pGameInstance->Key_Down(VK_LSHIFT))
            m_pTransformCom->Go_Right(fTimeDelta * 15.5f, m_pNavigationCom);
    }

    if (m_pGameInstance->Get_DIKeyState(DIK_W) & 0x80)
    {
        m_pTransformCom->Go_StraightY(fTimeDelta, m_pNavigationCom, m_bJump);

        if (m_pGameInstance->Key_Down(VK_LSHIFT))
            m_pTransformCom->Go_StraightY(fTimeDelta * 15.5f, m_pNavigationCom);      
    }

    if (m_pGameInstance->Get_DIKeyState(DIK_S) & 0x80)
    {
        m_pTransformCom->Go_BackwardY(fTimeDelta, m_pNavigationCom, m_bJump);

        if (m_pGameInstance->Key_Down(VK_LSHIFT))
            m_pTransformCom->Go_BackwardY(fTimeDelta * 15.5f, m_pNavigationCom);
    }

    if (m_pGameInstance->Key_Down('1'))
    {
        if (m_eWeaponIDArray[ONE] == WEAPON_END)
            return;

        m_eWeaponID = m_eWeaponIDArray[ONE];
        m_eSlotID = ONE;
        m_bWeaponChange = true;
    }

    if (m_pGameInstance->Key_Down('2'))
    {
        if (m_eWeaponIDArray[TWO] == WEAPON_END)
            return;

        m_eWeaponID = m_eWeaponIDArray[TWO];
        m_eSlotID = TWO;
        m_bWeaponChange = true;
    }

    if (m_pGameInstance->Key_Down('3'))
    {
        m_eWeaponID = PISTOL;
        m_bWeaponChange = true;
    }

    if (m_pGameInstance->Key_Down('4'))
    {
        m_eWeaponID = SHURIKEN;
        m_bWeaponChange = true;
    }
}

void CPlayer::Player_Jump(_float fTimeDelta)
{
    _float fHeight = m_pNavigationCom->Navi_Height(m_pTransformCom);

    if (!m_bJump)
    {
        m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSetY(m_pTransformCom->Get_State(CTransform::STATE_POSITION), fHeight + 1.0f));
    }

    if (m_pGameInstance->Key_Down(VK_SPACE)|| m_bJump)
    {

        m_pTransformCom->Go_Up(fTimeDelta);
        _uint a = m_pNavigationCom->Init_Index(m_pTransformCom);
        m_bJump = true;
    }
    
    if (m_pTransformCom->Gravity(0.2f, fHeight + 1.0f, fTimeDelta) && m_bJump)
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
            Desc.iNumBullet = 8;
            Desc.iMaxBullet = 999999999;
            Desc.fAttackSpeed = 0.25f;
            __super::Add_PartObject(PART_WEAPON, TEXT("Prototype_GameObject_Weapon_Player_Pistol"), &Desc);

            break;

        case SHURIKEN:
            Desc.iNumBullet = 1;
            Desc.iNumBullet = 60;
            Desc.fAttackSpeed = 0.25f;
            __super::Add_PartObject(PART_WEAPON, TEXT("Prototype_GameObject_Weapon_Player_Shuriken"), &Desc);

            break;

        case SNIPER:
            
            Desc.iNumBullet = 8;
            Desc.iMaxBullet = 80;
            Desc.fAttackSpeed = 0.25f;
            __super::Add_PartObject(PART_WEAPON, TEXT("Prototype_GameObject_Weapon_Player_Sniper"), &Desc);

            break;

        case BOOMGUN:
            Desc.iNumBullet = 30;
            Desc.iMaxBullet = 120;
            Desc.fAttackSpeed = 0.125f;
            __super::Add_PartObject(PART_WEAPON, TEXT("Prototype_GameObject_Weapon_Player_BoomGun"), &Desc);
            break;

        default:
            break;
        }

        
        m_bWeaponChange = false;
    }
}

void CPlayer::Dash(_float fTimeDelta)
{
    m_pTransformCom->Go_StraightY(fTimeDelta);

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
    Safe_Release(m_pColliderCom);
    Safe_Release(m_pNavigationCom);

    __super::Free();
}
