#include "stdafx.h"

#include "Skill_Dragon_Bubble.h"
#include "GameInstance.h"

#include "Player.h"

CSkill_Dragon_Bubble::CSkill_Dragon_Bubble(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CSkill_Monster{ pDevice , pContext }
{
}

CSkill_Dragon_Bubble::CSkill_Dragon_Bubble(const CSkill_Dragon_Bubble& Prototype)
    :CSkill_Monster{ Prototype }
{
}

HRESULT CSkill_Dragon_Bubble::Initialize_Prototype()
{
    if (FAILED(__super::Initialize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CSkill_Dragon_Bubble::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(Ready_Components()))
        return E_FAIL;

    Safe_AddRef(m_pOriginTransform);

    Offset_Rotation({ 0.f, 0.f, 0.f });
    m_vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

    m_pTransformCom->Set_Scaled( 1.f, 1.f, 1.f );
    m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSetY(m_pTransformCom->Get_State(CTransform::STATE_POSITION), 3.f));
    m_pTransformCom->Rotation({0.f , 1.f ,0.f},XMConvertToRadians(m_pOriginTransform->Look_Degree() + m_iNumSkill * 5.f -15.f));

    return S_OK;
}

void CSkill_Dragon_Bubble::Priority_Update(_float fTimeDelta)
{
    m_fAnimFrame += 6.0f * fTimeDelta;

    if (m_fAnimFrame >= 3.f)
        m_fAnimFrame = 0.f;
}

void CSkill_Dragon_Bubble::Update(_float fTimeDelta)
{
    m_pColliderCom->Update(m_pTransformCom->Get_WorldMatrix());
    
    m_pTransformCom->Go_Up(fTimeDelta);

    if (m_pTransformCom->Gravity(Get_Random(0.01f, 0.5f), 0.0f, fTimeDelta))
    {
        m_bDead = true;
        SKILL_DESC Desc = {};
        Desc.pOriginTransform = m_pTransformCom;
        Desc.iSkillDamage = 5.f;
        Desc.iNumSkill = 1;

        m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Skill_Dragon_Floor"), TEXT("Layer_Skill_Dragon_Floor"),&Desc);
    }
    m_pTransformCom->Go_Straight(fTimeDelta);
}

void CSkill_Dragon_Bubble::Late_Update(_float fTimeDelta)
{
    CCollider* pTargetCollider = dynamic_cast<CCollider*>(m_pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_Player"), TEXT("Com_Collider"), 0));
    CPlayer* pPlayer = dynamic_cast<CPlayer*>(m_pGameInstance->Get_Layer(LEVEL_GAMEPLAY, TEXT("Layer_Player")).front());
   
    if (m_pColliderCom->isCollision(pTargetCollider))
    {
        pPlayer->Set_Damage(-5.f);
        m_bDead = true;
    }
#ifdef _DEBUG
    m_pGameInstance->Add_DebugComponent(m_pColliderCom);
#endif

    m_pGameInstance->Add_RenderObject(CRenderer::RG_BLEND, this);
}

HRESULT CSkill_Dragon_Bubble::Render()
{
    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Begin(1)))
        return E_FAIL;

    if (FAILED(m_pVIBufferCom->Bind_Buffers()))
        return E_FAIL;

    if (FAILED(m_pVIBufferCom->Render()))
        return E_FAIL;


    return S_OK;
}

HRESULT CSkill_Dragon_Bubble::Ready_Components()
{
    /* For.Com_Shader */
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxPosTex"),
        TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
        return E_FAIL;

    /* For.Com_Model */
    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Skill_Dragon_Bubble"),
        TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
        return E_FAIL;

    /* For.Com_VIBuffer */
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"),
        TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
        return E_FAIL;

    /* For.Com_ColliderSphere */
    CBounding_Sphere::BOUNDING_SPHERE_DESC   BoundingDesc{};

    BoundingDesc.fRadius = 0.5f;
    BoundingDesc.vCenter = _float3(0.f, BoundingDesc.fRadius * 0.5f, 0.0f);

    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_Sphere"),
        TEXT("Com_ColliderSphere"), reinterpret_cast<CComponent**>(&m_pColliderCom), &BoundingDesc)))
        return E_FAIL;

    return S_OK;
}

HRESULT CSkill_Dragon_Bubble::Bind_ShaderResources()
{
    if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
        return E_FAIL; 

    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
        return E_FAIL;

    if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", (_uint)m_fAnimFrame)))
        return E_FAIL;

    return S_OK;
}

void CSkill_Dragon_Bubble::Destroy(_float fTimeDelta)
{
    //m_fDestroyTimer += fTimeDelta;
    //
    //if (m_fDestroyTimer > 5.f)
    //    m_bDead = true;

}


CSkill_Dragon_Bubble* CSkill_Dragon_Bubble::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CSkill_Dragon_Bubble* pInstance = new CSkill_Dragon_Bubble(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX(TEXT("Failed to Created : CSkill_Dragon_Bubble"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CSkill_Dragon_Bubble::Clone(void* pArg)
{
    CSkill_Dragon_Bubble* pInstance = new CSkill_Dragon_Bubble(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX(TEXT("Failed to Cloned : CSkill_Dragon_Bubble"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CSkill_Dragon_Bubble::Free()
{
    Safe_Release(m_pOriginTransform);
    Safe_Release(m_pShaderCom);
    Safe_Release(m_pVIBufferCom);
    Safe_Release(m_pTextureCom);
    Safe_Release(m_pColliderCom);

    __super::Free();
}
