#include "stdafx.h"

#include "Skill_Archer_Bullet.h"
#include "GameInstance.h"   

#include "Player.h"

CSkill_Archer_Bullet::CSkill_Archer_Bullet(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CSkill_Monster{ pDevice , pContext }
{
}

CSkill_Archer_Bullet::CSkill_Archer_Bullet(const CSkill_Archer_Bullet& Prototype)
    :CSkill_Monster{ Prototype }
{
}

HRESULT CSkill_Archer_Bullet::Initialize_Prototype()
{
    if (FAILED(__super::Initialize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CSkill_Archer_Bullet::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(Ready_Components()))
        return E_FAIL;

    SKILL_DESC* Desc = static_cast<SKILL_DESC*>(pArg);
    m_pTargetTransform = Desc->pTargetTransform;
    m_iSkillDamage = Desc->iSkillDamage;

    Safe_AddRef(m_pTargetTransform);

    m_pTransformCom->Set_Scaled(3.f, 0.25f, 0.25f);

    m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSetY(m_pTransformCom->Get_State(CTransform::STATE_POSITION) ,1.f));
    m_vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

    CTransform* pPlayerTransform = dynamic_cast<CTransform*>(m_pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_Player"), TEXT("Com_Transform")));

    m_vOffsetRandomPos = { Get_Random(0, 2) - 1.f , Get_Random(0, 2) - 1.f,Get_Random(0, 2) - 1.0f };

    m_vPlayerPos = pPlayerTransform->Get_State(CTransform::STATE_POSITION);
    m_vPlayerPos += m_vOffsetRandomPos;
        //XMVectorSetX(pPlayerTransform->Get_State(CTransform::STATE_POSITION) , m_vOffsetRandomPos.x);
    
    //네비게이션 메쉬 밖으로 나가면 delete? good 
    return S_OK;
}

void CSkill_Archer_Bullet::Priority_Update(_float fTimeDelta)
{
}

void CSkill_Archer_Bullet::Update(_float fTimeDelta)
{
    m_pColliderCom->Update(m_pTransformCom->Get_WorldMatrix());
    
    _vector	vPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
    _vector vMoveDir = XMVector4Normalize(m_vPlayerPos - m_vPos);   

    vPosition += vMoveDir * 10.f * fTimeDelta;
    m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPosition);

    Destroy(fTimeDelta);
}

void CSkill_Archer_Bullet::Late_Update(_float fTimeDelta)
{
    CCollider* pTargetCollider = dynamic_cast<CCollider*>(m_pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_Player"), TEXT("Com_Collider"), 0));
    CPlayer* pPlayer = dynamic_cast<CPlayer*>(m_pGameInstance->Get_Layer(LEVEL_GAMEPLAY, TEXT("Layer_Player")).front());

    if (m_pColliderCom->isCollision(pTargetCollider))
    {
        pPlayer->Set_Damage(-5.f);
        m_bDead = true;
    }

    m_pGameInstance->Add_RenderObject(CRenderer::RG_BLEND, this);
}

HRESULT CSkill_Archer_Bullet::Render()
{
#ifdef _DEBUG
    m_pColliderCom->Render();
#endif

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

HRESULT CSkill_Archer_Bullet::Ready_Components()
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

    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_Sphere"),
        TEXT("Com_ColliderSphere"), reinterpret_cast<CComponent**>(&m_pColliderCom), &BoundingDesc)))
        return E_FAIL;

    return S_OK;
}

HRESULT CSkill_Archer_Bullet::Bind_ShaderResources()
{
    if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
        return E_FAIL;

    if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", 3)))
        return E_FAIL;

    return S_OK;
}

void CSkill_Archer_Bullet::Destroy(_float fTimeDelta)
{
    m_fDestroyTimer += fTimeDelta;

    if (m_fDestroyTimer > 5.f)
        m_bDead = true;

}


CSkill_Archer_Bullet* CSkill_Archer_Bullet::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CSkill_Archer_Bullet* pInstance = new CSkill_Archer_Bullet(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX(TEXT("Failed to Created : CSkill_Archer_Bullet"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CSkill_Archer_Bullet::Clone(void* pArg)
{
    CSkill_Archer_Bullet* pInstance = new CSkill_Archer_Bullet(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX(TEXT("Failed to Cloned : CSkill_Archer_Bullet"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CSkill_Archer_Bullet::Free()
{
    Safe_Release(m_pTargetTransform);
    Safe_Release(m_pShaderCom);
    Safe_Release(m_pVIBufferCom);
    Safe_Release(m_pTextureCom);
    Safe_Release(m_pColliderCom);

    __super::Free();
}
