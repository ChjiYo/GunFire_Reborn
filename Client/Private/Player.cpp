#include "stdafx.h"

#include "Player.h"
#include "GameInstance.h"
#include "Camera.h"

#include "FSM_Player_Attack.h"
#include "FSM_Player_Dash.h"

CPlayer::CPlayer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CGameObject { pDevice  , pContext }
{
}

CPlayer::CPlayer(const CPlayer& Prototype)
    :CGameObject{ Prototype }
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
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    PLAYER_DESC* Desc = static_cast<PLAYER_DESC*>(pArg);
   
    Player_Desc.iHp = Desc->iHp;
    Player_Desc.iMaxHp = Desc->iMaxHp;
    Player_Desc.iShield = Desc->iShield;
    Player_Desc.iMaxShield = Desc->iMaxShield;

    m_pCamTransform = Desc->pTargetTransform;

    Safe_AddRef(m_pCamTransform);

    if (FAILED(Ready_Components()))
        return E_FAIL;

    if (FAILED(Ready_State()))
        return E_FAIL;

    return S_OK;
}

void CPlayer::Priority_Update(_float fTimeDelta)
{
    if (fTimeDelta == 0.f)
        return;

    Position();
    Key_State();

    m_pModelCom->Play_Animation(fTimeDelta);
}

void CPlayer::Update(_float fTimeDelta)
{
    m_pColliderCom->Update(m_pTransformCom->Get_WorldMatrix());
}

void CPlayer::Late_Update(_float fTimeDelta)
{
    m_pGameInstance->Add_RenderObject(CRenderer::RG_NONBLEND, this);
}

HRESULT CPlayer::Render()
{
    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;

    _uint	iNumMeshes = m_pModelCom->Get_NumMeshes();

    for (size_t i = 0; i < iNumMeshes; i++)
    {
        if (FAILED(m_pModelCom->Bind_MaterialSRV(m_pShaderCom, aiTextureType_DIFFUSE, "g_DiffuseTexture", i)))
            return E_FAIL;

        //if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, "g_NormalTexture", i, aiTextureType_NORMALS)))
        //	return E_FAIL;

        if (FAILED(m_pModelCom->Bind_BoneMatrices(m_pShaderCom, "g_BoneMatrices", (_uint)i)))
            return E_FAIL;

        m_pShaderCom->Begin(0);
        
        m_pModelCom->Render(i);
    }

#ifdef _DEBUG
    m_pColliderCom->Render();
#endif

    return S_OK;
}

HRESULT CPlayer::Ready_Components()
{
    /* For.Com_Shader */
    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxAnimMesh"),
        TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
        return E_FAIL;

    /* For.Com_Model */
    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Player"),
        TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
        return E_FAIL;

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

HRESULT CPlayer::Bind_ShaderResources()
{
    if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
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

void CPlayer::Key_State()
{
    if (m_pGameInstance->Key_Down(VK_LBUTTON))
    {
        m_pModelCom->SetUp_Animation(1, false);
        m_pState->Set_State(TEXT("Player_State_Attack"));
    }

    if(m_pGameInstance->Key_Down(VK_LSHIFT))
        m_pState->Set_State(TEXT("Player_State_Dash"));

}

void CPlayer::Position()
{
    _vector vRight = m_pCamTransform->Get_State(CTransform::STATE_RIGHT);
    _vector vUp = m_pCamTransform->Get_State(CTransform::STATE_UP);
    _vector vLook = m_pCamTransform->Get_State(CTransform::STATE_LOOK);

    _vector vPlayerPosition = m_pCamTransform->Get_State(CTransform::STATE_POSITION);

    _vector vWeaponRight = vRight * 0.8f;
    _vector vWeaponUp = vUp * -0.7f;
    _vector vWeaponLook = vLook * 1.f;
    _vector vWeaponPosition = vPlayerPosition + vWeaponRight + vWeaponUp + vWeaponLook;

    m_pTransformCom->Set_State(CTransform::STATE_POSITION, vWeaponPosition);

    m_pTransformCom->LookAt(vPlayerPosition + vLook * 15.f);
    m_pTransformCom->Radian_Turn(m_pTransformCom->Get_State(CTransform::STATE_UP), XMConvertToRadians(335.f));
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
    Safe_Release(m_pShaderCom);
    Safe_Release(m_pModelCom);
    Safe_Release(m_pCamTransform);
    Safe_Release(m_pState);
    Safe_Release(m_pColliderCom);

    __super::Free();
}
