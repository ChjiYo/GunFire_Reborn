#include "stdafx.h"

#include "Skill_Mon_Boom.h"
#include "GameInstance.h"

#include "Player.h"

CSkill_Mon_Boom::CSkill_Mon_Boom(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CSkill_Monster{ pDevice , pContext }
{
}

CSkill_Mon_Boom::CSkill_Mon_Boom(const CSkill_Mon_Boom& Prototype)
    :CSkill_Monster{ Prototype }
{
}

HRESULT CSkill_Mon_Boom::Initialize_Prototype()
{
    if (FAILED(__super::Initialize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CSkill_Mon_Boom::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(Ready_Components()))
        return E_FAIL;

    Safe_AddRef(m_pOriginTransform);
    m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_pOriginTransform->Get_State(CTransform::STATE_POSITION));
    m_pTransformCom->Set_Scaled(2.f, 2.f, 2.f);

    return S_OK;
}

void CSkill_Mon_Boom::Priority_Update(_float fTimeDelta)
{
    m_fAnimFrame += fTimeDelta;

    m_fAnimFrame += 30.0f * fTimeDelta;

    if (m_fAnimFrame >= 15.f)
    {
        m_bDead = true;
        m_fAnimFrame = 0.f;
    }
    
    CTransform* pPlayerTransform = dynamic_cast<CTransform*>(m_pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_Player"), TEXT("Com_Transform")));
    m_pTransformCom->LookAt(XMVectorSetY(pPlayerTransform->Get_State(CTransform::STATE_POSITION) , 0.f));
}

void CSkill_Mon_Boom::Update(_float fTimeDelta)
{
    m_pColliderCom->Update(m_pTransformCom->Get_WorldMatrix());

}

void CSkill_Mon_Boom::Late_Update(_float fTimeDelta)
{
    CCollider* pTargetCollider = dynamic_cast<CCollider*>(m_pGameInstance->Get_Component(m_pGameInstance->GetLoadingLevelIndex(), TEXT("Layer_Player"), TEXT("Com_Collider"), 0));
    CPlayer* pPlayer = dynamic_cast<CPlayer*>(m_pGameInstance->Get_Layer(m_pGameInstance->GetLoadingLevelIndex(), TEXT("Layer_Player")).front());

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

HRESULT CSkill_Mon_Boom::Render()
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

HRESULT CSkill_Mon_Boom::Ready_Components()
{
    /* For.Com_Shader */
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxPosTex"),
        TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
        return E_FAIL;

    /* For.Com_Model */
    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Boom_Fire"),
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

HRESULT CSkill_Mon_Boom::Bind_ShaderResources()
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

void CSkill_Mon_Boom::Destroy(_float fTimeDelta)
{


}


CSkill_Mon_Boom* CSkill_Mon_Boom::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CSkill_Mon_Boom* pInstance = new CSkill_Mon_Boom(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX(TEXT("Failed to Created : CSkill_Mon_Boom"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CSkill_Mon_Boom::Clone(void* pArg)
{
    CSkill_Mon_Boom* pInstance = new CSkill_Mon_Boom(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX(TEXT("Failed to Cloned : CSkill_Mon_Boom"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CSkill_Mon_Boom::Free()
{
    Safe_Release(m_pOriginTransform);
    Safe_Release(m_pShaderCom);
    Safe_Release(m_pVIBufferCom);
    Safe_Release(m_pTextureCom);
    Safe_Release(m_pColliderCom);

    __super::Free();
}
