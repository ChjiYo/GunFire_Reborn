#include "stdafx.h"

#include "Skill_Dragon_Floor.h"
#include "GameInstance.h"

#include "Player.h"

CSkill_Dragon_Floor::CSkill_Dragon_Floor(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CSkill_Monster{ pDevice , pContext }
{
}

CSkill_Dragon_Floor::CSkill_Dragon_Floor(const CSkill_Dragon_Floor& Prototype)
    :CSkill_Monster{ Prototype }
{
}

HRESULT CSkill_Dragon_Floor::Initialize_Prototype()
{
    if (FAILED(__super::Initialize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CSkill_Dragon_Floor::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(Ready_Components()))
        return E_FAIL;

    Safe_AddRef(m_pOriginTransform);
    
    m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSetY(m_pOriginTransform->Get_State(CTransform::STATE_POSITION),0.f));
    m_pTransformCom->Set_Scaled(5.f, 5.f, 1.f);
    m_pTransformCom->Rotation({ 1.f, 0.f , 0.f }, XMConvertToRadians(90.f));

    return S_OK;
}

void CSkill_Dragon_Floor::Priority_Update(_float fTimeDelta)
{
    m_fDestoryTimer += fTimeDelta;
    m_fTickDamageTimer += fTimeDelta;
}

void CSkill_Dragon_Floor::Update(_float fTimeDelta)
{
    m_pColliderCom->Update(m_pTransformCom->Get_WorldMatrix());

    Destroy(fTimeDelta);
}

void CSkill_Dragon_Floor::Late_Update(_float fTimeDelta)
{
    CCollider* pTargetCollider = dynamic_cast<CCollider*>(m_pGameInstance->Get_Component(m_pGameInstance->GetLoadingLevelIndex(), TEXT("Layer_Player"), TEXT("Com_Collider"), 0));
    CPlayer* pPlayer = dynamic_cast<CPlayer*>(m_pGameInstance->Get_Layer(m_pGameInstance->GetLoadingLevelIndex(), TEXT("Layer_Player")).front());

    if (m_pColliderCom->isCollision(pTargetCollider) && m_fTickDamageTimer >= 0.5f)
    {
        m_fTickDamageTimer = 0.f;
        pPlayer->Set_Damage(-5.f);
    }
#ifdef _DEBUG
    m_pGameInstance->Add_DebugComponent(m_pColliderCom);
#endif

    m_pGameInstance->Add_RenderObject(CRenderer::RG_BLEND, this);
}

HRESULT CSkill_Dragon_Floor::Render()
{
    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Begin(2)))
        return E_FAIL;

    if (FAILED(m_pVIBufferCom->Bind_Buffers()))
        return E_FAIL;

    if (FAILED(m_pVIBufferCom->Render()))
        return E_FAIL;


    return S_OK;
}

HRESULT CSkill_Dragon_Floor::Ready_Components()
{
    /* For.Com_Shader */
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxPosTex"),
        TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
        return E_FAIL;

    /* For.Com_Model */
    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Skill_Dragon_Floor"),
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

    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_Sphere"),
        TEXT("Com_ColliderSphere"), reinterpret_cast<CComponent**>(&m_pColliderCom), &BoundingDesc)))
        return E_FAIL;

    return S_OK;
}

HRESULT CSkill_Dragon_Floor::Bind_ShaderResources()
{
    if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
        return E_FAIL;

    if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", 0)))
        return E_FAIL;

    return S_OK;
}

void CSkill_Dragon_Floor::Destroy(_float fTimeDelta)
{
    if (m_fDestoryTimer >= 2.f)
    {
        m_fDestoryTimer = 0.f;
        m_bDead = true;
    }

}


CSkill_Dragon_Floor* CSkill_Dragon_Floor::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CSkill_Dragon_Floor* pInstance = new CSkill_Dragon_Floor(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX(TEXT("Failed to Created : CSkill_Dragon_Floor"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CSkill_Dragon_Floor::Clone(void* pArg)
{
    CSkill_Dragon_Floor* pInstance = new CSkill_Dragon_Floor(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX(TEXT("Failed to Cloned : CSkill_Dragon_Floor"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CSkill_Dragon_Floor::Free()
{
    Safe_Release(m_pOriginTransform);
    Safe_Release(m_pShaderCom);
    Safe_Release(m_pVIBufferCom);
    Safe_Release(m_pTextureCom);
    Safe_Release(m_pColliderCom);

    __super::Free();
}
