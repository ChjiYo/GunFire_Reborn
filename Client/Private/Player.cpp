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
#include "Data_Manager.h"

CPlayer::CPlayer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CContainerObject{ pDevice  , pContext }
    , m_pDataMgr{ CData_Manager::Get_Instance() }
{
}

CPlayer::CPlayer(const CPlayer& Prototype)
    :CContainerObject{ Prototype }
    , m_pDataMgr{ CData_Manager::Get_Instance() }
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

    m_eSlotID = THREE;
    m_eWeaponID = PISTOL;
    m_eWeaponIDArray[2] = PISTOL;

    switch (m_pGameInstance->GetLoadingLevelIndex())
    {
    case LEVEL_GAMEPLAY: 
        m_pTransformCom->Set_State(CTransform::STATE_POSITION, { 6.f , 1.5f , 2.f, 1.f });
        break;

    case LEVEL_BOSS:
        m_pTransformCom->Set_State(CTransform::STATE_POSITION, { 106.f , 1.0f , 70.f, 1.f });
        break;
    }

    m_pNavigationCom->Init_Index(m_pTransformCom);


    LIGHT_DESC			LightDesc{};

    ZeroMemory(&LightDesc, sizeof(LIGHT_DESC));
    LightDesc.eType = LIGHT_DESC::TYPE_POINT;
    LightDesc.vPosition = _float4(0.f, 0.f, 0.f, 1.f);
    LightDesc.fRange = 15.f;
    LightDesc.vDiffuse = _float4(1.0f, 0.0f, 0.0f, 0.75f);
    LightDesc.vAmbient = _float4(0.25f, 0.25f, 0.f, 1.f);
    LightDesc.vSpecular = _float4(0.f, 0.f, 0.f, 1.f);
    
    if (FAILED(m_pGameInstance->Add_Light(LightDesc)))
        return E_FAIL;

    m_pLight = m_pGameInstance->Get_Light().back();
    
    _vector vTmp = { 0 , 0.f , 0.f,1.f };
    _vector vSrc = XMVectorZero();

    _vector vDes =  XMVectorEqual(vTmp, vSrc);
    _bool bResult = XMVector4EqualInt(vDes, XMVectorTrueInt());   

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

    if (m_bHit)
    {
        m_fHitTimer += fTimeDelta * 0.5f;

        if (m_fHitTimer >= 0.5f)
        {
            m_fHitTimer = 0.f;
            m_HitCnt++;
        }

        if (m_HitCnt >= 3)
        {
            m_HitCnt = 0;
            m_fHitTimer = 0.f;
            m_bHit = false;
        }

    }
    __super::Priority_Update(fTimeDelta);
}

void CPlayer::Update(_float fTimeDelta)
{
    __super::Update(fTimeDelta);

    if (m_pGameInstance->Key_Down('J'))
    {
        Player_Desc.iHp -= 20;
    }

    m_pLight->Set_LightPos(m_pTransformCom->Get_State(CTransform::STATE_POSITION));
    m_pColliderCom->Update(m_pTransformCom->Get_WorldMatrix());

    ShieldRecovery();
    Player_Weapon_Change();

    _vector vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);  
}

void CPlayer::Late_Update(_float fTimeDelta)
{
    __super::Late_Update(fTimeDelta);

    Key_State(fTimeDelta);

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
        m_eSlotID = THREE;
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
        if (m_Parts[PART_WEAPON] != nullptr)
        {
            _uint iMaxBullet = dynamic_cast<CWeapon_Player*>(m_Parts[PART_WEAPON])->Get_MaxBullet();
            _uint iCurrBullet = dynamic_cast<CWeapon_Player*>(m_Parts[PART_WEAPON])->Get_CurrentBullet();
            m_pDataMgr->SaveBullet(m_ePrevWeaponID, iMaxBullet,iCurrBullet );
        }

        Safe_Release(__super::Get_Parts().front());
        __super::Get_Parts().clear();

        CWeapon_Player::PLAYER_WEAPON_DESC Desc{};
        Desc.pParentMatrix = m_pTransformCom->Get_WorldMatrixPtr();
        Desc.pParentState = 0x00000000;
        Desc.pTargetTransform = m_pTransformCom;
        switch (m_eWeaponID)
        {
        case PISTOL:
            Desc.iNumBullet = m_pDataMgr->Get_CurrBullet(PISTOL);
            Desc.iMaxBullet = 999999999;
            Desc.fAttackSpeed = 0.25f;
            Desc.iBulletDamage = m_vDamage[m_eWeaponID] + m_vUpgreadCnt[m_eWeaponID] * 10;
            __super::Add_PartObject(PART_WEAPON, TEXT("Prototype_GameObject_Weapon_Player_Pistol"), &Desc);

            break;

        case SHURIKEN:
            Desc.iNumBullet = 1;
            Desc.iMaxBullet = 60;
            Desc.fAttackSpeed = 0.25f;
            Desc.iBulletDamage = m_vDamage[m_eWeaponID] + m_vUpgreadCnt[m_eWeaponID] * 10;
            __super::Add_PartObject(PART_WEAPON, TEXT("Prototype_GameObject_Weapon_Player_Shuriken"), &Desc);

            break;

        case SNIPER:
            
            Desc.iNumBullet = m_pDataMgr->Get_CurrBullet(SNIPER);;
            Desc.iMaxBullet = m_pDataMgr->Get_MaxBullet(SNIPER);;
            Desc.fAttackSpeed = 0.25f;
            Desc.iBulletDamage = m_vDamage[m_eWeaponID] + m_vUpgreadCnt[m_eWeaponID] * 10;
            __super::Add_PartObject(PART_WEAPON, TEXT("Prototype_GameObject_Weapon_Player_Sniper"), &Desc);

            break;

        case BOOMGUN:
            Desc.iNumBullet = 30;
            Desc.iMaxBullet = 90;
            Desc.fAttackSpeed = 0.125f;
            Desc.iBulletDamage = m_vDamage[m_eWeaponID] + m_vUpgreadCnt[m_eWeaponID] * 10;
            __super::Add_PartObject(PART_WEAPON, TEXT("Prototype_GameObject_Weapon_Player_BoomGun"), &Desc);
            break;

        default:
            break;
        }

        m_ePrevWeaponID = m_eWeaponID;
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
