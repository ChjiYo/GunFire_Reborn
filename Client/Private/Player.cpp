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

    m_pCamTransform = Desc->pTargetTransform; // clear

    if (FAILED(Ready_PartObjects()))
        return E_FAIL;

    if (FAILED(Ready_Components()))
        return E_FAIL;

    if (FAILED(Ready_State()))
        return E_FAIL;


    Safe_AddRef(m_pCamTransform);

    return S_OK;
}

void CPlayer::Priority_Update(_float fTimeDelta)
{
    if (fTimeDelta == 0.f)
        return;

    Key_State();

    __super::Priority_Update(fTimeDelta);
}

void CPlayer::Update(_float fTimeDelta)
{
    m_pColliderCom->Update(m_pTransformCom->Get_WorldMatrix());

    __super::Update(fTimeDelta);
}

void CPlayer::Late_Update(_float fTimeDelta)
{
    __super::Late_Update(fTimeDelta);

    m_pGameInstance->Add_RenderObject(CRenderer::RG_NONBLEND, this);
}

HRESULT CPlayer::Render()
{
#ifdef _DEBUG
    m_pColliderCom->Render();
#endif

    return S_OK;
}

HRESULT CPlayer::Ready_PartObjects()
{
    CWeapon_Player_Pistol::WEAPON_DESC  Desc{};
    Desc.pParentMatrix = m_pTransformCom->Get_WorldMatrixPtr();
    Desc.pParentState = 0x00000000;
    Desc.pTargetTransform = m_pCamTransform;

    if (FAILED(__super::Add_PartObject(PART_WEAPON, TEXT("Prototype_GameObject_Weapon_Player_Pistol"), &Desc)))
        return E_FAIL;

    return S_OK;
}

HRESULT CPlayer::Ready_Components()
{
    /* For.Com_State */
    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_State"),
        TEXT("Com_State"), reinterpret_cast<CComponent**>(&m_pState))))
        return E_FAIL;

   CBounding_AABB::BOUNDING_AABB_DESC	BoundingDesc{};
   
   BoundingDesc.vExtents = _float3(0.5f, 0.5f, 0.5f);
   BoundingDesc.vCenter = _float3(0.f, BoundingDesc.vExtents.y, 0.f);   if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_AABB"),
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
void CPlayer::Key_State()
{
    if (m_pGameInstance->Key_Down(VK_LBUTTON))
        m_pState->Set_State(TEXT("Player_State_Attack"));
    

    if(m_pGameInstance->Key_Down(VK_LSHIFT))
        m_pState->Set_State(TEXT("Player_State_Dash"));
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
    Safe_Release(m_pCamTransform);
    Safe_Release(m_pState);
    Safe_Release(m_pColliderCom);

    __super::Free();
}
