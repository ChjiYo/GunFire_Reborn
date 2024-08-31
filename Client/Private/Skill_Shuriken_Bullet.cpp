#include "stdafx.h"

#include "Skill_Shuriken_Bullet.h"
#include "GameInstance.h"   

CSkill_Shuriken_Bullet::CSkill_Shuriken_Bullet(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CSkill_Player{ pDevice , pContext }
{
}

CSkill_Shuriken_Bullet::CSkill_Shuriken_Bullet(const CSkill& Prototype)
    :CSkill_Player{ Prototype }
{
}

HRESULT CSkill_Shuriken_Bullet::Initialize_Prototype()
{
    if (FAILED(__super::Initialize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CSkill_Shuriken_Bullet::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(Ready_Components()))
        return E_FAIL;

    Safe_AddRef(m_pOriginTransform);

    Set_Position({ 0.f , 0.25f, 1.f });

    m_vTargetLook = m_pOriginTransform->Get_State(CTransform::STATE_LOOK);
    m_pTransformCom->Set_Scaled(3.f, 0.25f, 1.f);

    //네비게이션 메쉬 밖으로 나가면 delete? good 
    return S_OK;
}

void CSkill_Shuriken_Bullet::Priority_Update(_float fTimeDelta)
{
    //_vector vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
  
}

void CSkill_Shuriken_Bullet::Update(_float fTimeDelta)
{
    m_pColliderCom->Update(m_pTransformCom->Get_WorldMatrix());

    _vector	vPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

    vPosition += m_vTargetLook * 50.f * fTimeDelta;
    m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPosition);

    Destroy(fTimeDelta);
}

void CSkill_Shuriken_Bullet::Late_Update(_float fTimeDelta)
{
    __super::Late_Update(fTimeDelta);
    m_pGameInstance->Add_RenderObject(CRenderer::RG_BLEND, this);
}

HRESULT CSkill_Shuriken_Bullet::Render()
{


    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Begin(0)))
        return E_FAIL;

    if (FAILED(m_pVIBufferCom->Bind_Buffers()))
        return E_FAIL;

    if (FAILED(m_pVIBufferCom->Render()))
        return E_FAIL;


    return S_OK;
}

HRESULT CSkill_Shuriken_Bullet::Ready_Components()
{
    /* For.Com_Shader */
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxPosTex"),
        TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
        return E_FAIL;

    /* For.Com_Model */
    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Skill_Bullet_Red"),
        TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
        return E_FAIL;

    /* For.Com_VIBuffer */
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"),
        TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
        return E_FAIL;

    /* For.Com_ColliderSphere */
    CBounding_Sphere::BOUNDING_SPHERE_DESC   BoundingDesc{};

    BoundingDesc.fRadius = 0.05f;
    BoundingDesc.vCenter = _float3(0.5f, BoundingDesc.fRadius, 0.0f);

    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_Sphere"),
        TEXT("Com_ColliderSphere"), reinterpret_cast<CComponent**>(&m_pColliderCom), &BoundingDesc)))
        return E_FAIL;

    return S_OK;
}

HRESULT CSkill_Shuriken_Bullet::Bind_ShaderResources()
{
    if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
        return E_FAIL;

    if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", 2)))
        return E_FAIL;

    return S_OK;
}

void CSkill_Shuriken_Bullet::Destroy(_float fTimeDelta)
{
    m_fDestroyTimer += fTimeDelta;

    if (m_fDestroyTimer > 5.f)
        m_bDead = true;

}

CSkill_Shuriken_Bullet* CSkill_Shuriken_Bullet::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CSkill_Shuriken_Bullet* pInstance = new CSkill_Shuriken_Bullet(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX(TEXT("Failed to Created : CSkill_Shuriken_Bullet"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CSkill_Shuriken_Bullet::Clone(void* pArg)
{
    CSkill_Shuriken_Bullet* pInstance = new CSkill_Shuriken_Bullet(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX(TEXT("Failed to Cloned : CSkill_CSkill_Shuriken_BulletPistol_Bullet"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CSkill_Shuriken_Bullet::Free()
{
    Safe_Release(m_pOriginTransform);
    Safe_Release(m_pShaderCom);
    Safe_Release(m_pVIBufferCom);
    Safe_Release(m_pTextureCom);
    Safe_Release(m_pColliderCom);

    __super::Free();
}
