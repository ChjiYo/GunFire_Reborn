#include "stdafx.h"

#include "Skill_Dragon_Breath.h"
#include "GameInstance.h"

#include "Bounding_AABB.h"

#include "Player.h"

CSkill_Dragon_Breath::CSkill_Dragon_Breath(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CSkill_Monster{ pDevice , pContext }
{
}

CSkill_Dragon_Breath::CSkill_Dragon_Breath(const CSkill_Dragon_Breath& Prototype)
    :CSkill_Monster{ Prototype }
{
    
}

HRESULT CSkill_Dragon_Breath::Initialize_Prototype()
{
    if (FAILED(__super::Initialize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CSkill_Dragon_Breath::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(Ready_Components()))
        return E_FAIL;

    SKILL_BREATH_DESC* Desc = static_cast<SKILL_BREATH_DESC*>(pArg);
    Breath_Desc.fAngle =  Desc->fAngle;
    
    Safe_AddRef(m_pOriginTransform);

    Offset_Rotation({ 0.f, 0.f, 0.f });
    
    m_pTransformCom->Rotation({ 0.f, 1.f, 0.f }, XMConvertToRadians(m_pOriginTransform->Look_Degree()));

    return S_OK;
}

void CSkill_Dragon_Breath::Priority_Update(_float fTimeDelta)
{
    m_fTickDamageTimer += fTimeDelta;
    m_fAngle += fTimeDelta * 15.f;
    m_fAnimFrame += fTimeDelta * 8.f * 2.f;

    if (m_fAnimFrame > 8.f)
        m_fAnimFrame = 0.f;

    if (m_fAngle > 45.f)
        m_fAngle = 0.f;
}

void CSkill_Dragon_Breath::Update(_float fTimeDelta)
{
    m_pColliderCom->Update(m_pTransformCom->Get_WorldMatrix());
    m_pTransformCom->Go_Straight(fTimeDelta);
    Destroy(fTimeDelta);
}

void CSkill_Dragon_Breath::Late_Update(_float fTimeDelta)
{
#ifdef _DEBUG
    m_pGameInstance->Add_DebugComponent(m_pColliderCom);
#endif

    _vector vTmp = m_pOriginTransform->Get_State(CTransform::STATE_POSITION);
    _vector vSrc = m_pOriginTransform->Get_State(CTransform::STATE_LOOK);;
    _float fDist = 15.f;

    m_pGameInstance->Add_RenderObject(CRenderer::RG_BLEND, this);
}

HRESULT CSkill_Dragon_Breath::Render()
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

HRESULT CSkill_Dragon_Breath::Ready_Components()
{
    /* For.Com_Shader */
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxPosTex"),
        TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
        return E_FAIL;

    /* For.Com_Model */
    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Skill_Dragon_Breath"),
        TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
        return E_FAIL;

    /* For.Com_VIBuffer */
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"),
        TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
        return E_FAIL;

    /* For.Com_ColliderSphere */
    CBounding_Sphere::BOUNDING_SPHERE_DESC BoundingDesc{};                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                               

    BoundingDesc.fRadius = 5.f;
    BoundingDesc.vCenter = { 0.f ,BoundingDesc.fRadius * 0.5f, 0.f };
    BoundingDesc.pObject = this;
    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_Sphere"),
        TEXT("Com_ColliderSphere"), reinterpret_cast<CComponent**>(&m_pColliderCom), &BoundingDesc)))
        return E_FAIL;

    return S_OK;
}

HRESULT CSkill_Dragon_Breath::Bind_ShaderResources()
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

void CSkill_Dragon_Breath::Destroy(_float fTimeDelta)
{
    m_fDestroyTimer += fTimeDelta;
    
    if (m_fDestroyTimer > 3.5f)
        m_bDead = true;

}

_bool CSkill_Dragon_Breath::IsCollider(_float fOriginX, _float fOriginZ, _float fTargetX, _float fTargetZ, _float fDistance)
{
    _float PosX = fOriginX + fDistance * cos(m_fAngle * (3.14f / 180.f));
    _float PosZ = fOriginZ - fDistance * sin(m_fAngle * (3.14f / 180.f));

    return _bool();
}


CSkill_Dragon_Breath* CSkill_Dragon_Breath::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CSkill_Dragon_Breath* pInstance = new CSkill_Dragon_Breath(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX(TEXT("Failed to Created : CSkill_Dragon_Breath"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CSkill_Dragon_Breath::Clone(void* pArg)
{
    CSkill_Dragon_Breath* pInstance = new CSkill_Dragon_Breath(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX(TEXT("Failed to Cloned : CSkill_Dragon_Breath"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CSkill_Dragon_Breath::Free()
{
    Safe_Release(m_pOriginTransform);
    Safe_Release(m_pShaderCom);
    Safe_Release(m_pVIBufferCom);
    Safe_Release(m_pTextureCom);
    Safe_Release(m_pColliderCom);

    __super::Free();                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 
}
