#include "stdafx.h"

#include "Skill_Pistol_Bullet.h"
#include "GameInstance.h"   

#include "Monster.h"

CSkill_Pistol_Bullet::CSkill_Pistol_Bullet(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CSkill_Player { pDevice , pContext }
{
}

CSkill_Pistol_Bullet::CSkill_Pistol_Bullet(const CSkill& Prototype)
    :CSkill_Player{ Prototype }
{
}

HRESULT CSkill_Pistol_Bullet::Initialize_Prototype()
{
    if(FAILED(__super::Initialize_Prototype()))
        return E_FAIL;

	return S_OK;
}

HRESULT CSkill_Pistol_Bullet::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(Ready_Components()))
        return E_FAIL;
   
    Safe_AddRef(m_pOriginTransform);

    m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_pOriginTransform->Get_State(CTransform::STATE_POSITION));

    m_vTargetLook = m_pOriginTransform->Get_State(CTransform::STATE_LOOK);
        
    m_pTransformCom->Set_Scaled(3.f, 0.25f, 0.25f);

    //네비게이션 메쉬 밖으로 나가면 delete? good 
    Add_Collider();

    m_pNavigationCom->Init_Index(m_pTransformCom);
    return S_OK;
}

void CSkill_Pistol_Bullet::Priority_Update(_float fTimeDelta)
{
}

void CSkill_Pistol_Bullet::Update(_float fTimeDelta)
{
    m_pColliderCom->Update(m_pTransformCom->Get_WorldMatrix());

   _vector	vPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

    vPosition += m_vTargetLook * 50.f * fTimeDelta;
    m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPosition);

    Destroy(fTimeDelta, vPosition);
}

void CSkill_Pistol_Bullet::Late_Update(_float fTimeDelta)
{
   list<CGameObject*> ObjList = m_pGameInstance->Get_Layer(LEVEL_GAMEPLAY,TEXT("Layer_Monster"));
        
    for (auto ColliderIter : HeadColliderList)
    {
        if (m_pColliderCom->isCollision(ColliderIter))
        {    
            dynamic_cast<CMonster*>(ColliderIter->Get_Object())->Set_Damage(-5.f);
            m_bDead = true;
        }
    }

    m_pGameInstance->Add_RenderObject(CRenderer::RG_BLEND, this);
}

HRESULT CSkill_Pistol_Bullet::Render()
{
#ifdef _DEBUG
        m_pColliderCom->Render();
#endif

    if(FAILED(Bind_ShaderResources()))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Begin(0)))
        return E_FAIL;

    if (FAILED(m_pVIBufferCom->Bind_Buffers()))
        return E_FAIL;

    if (FAILED(m_pVIBufferCom->Render()))
        return E_FAIL;


    return S_OK;
}

HRESULT CSkill_Pistol_Bullet::Ready_Components()
{
    if (FAILED(__super::Ready_Components()))
        return E_FAIL;

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

HRESULT CSkill_Pistol_Bullet::Bind_ShaderResources()
{
    if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
        return E_FAIL;

    if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", m_iTextureIndex)))
        return E_FAIL;

    return S_OK;
}

void CSkill_Pistol_Bullet::Destroy(_float fTimeDelta, _vector vPos)
{
    m_fDestroyTimer += fTimeDelta;

    if(m_fDestroyTimer > 5.f)
        m_bDead = true;

    if(!m_pNavigationCom->is_Move(vPos))
        m_bDead = true;
}

void CSkill_Pistol_Bullet::Add_Collider()
{
    int i = 0; 
    while (true)
    {
        CCollider* pTargetCollider = dynamic_cast<CCollider*>(m_pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_Monster"), TEXT("Com_Collider_Sphere"), i));

        if (pTargetCollider == nullptr)
            return;

        HeadColliderList.push_back(pTargetCollider);

        i++;
    }
}

CSkill_Pistol_Bullet* CSkill_Pistol_Bullet::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CSkill_Pistol_Bullet* pInstance = new CSkill_Pistol_Bullet(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX(TEXT("Failed to Created : CSkill_Pistol_Bullet"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CSkill_Pistol_Bullet::Clone(void* pArg)
{
    CSkill_Pistol_Bullet* pInstance = new CSkill_Pistol_Bullet(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX(TEXT("Failed to Cloned : CSkill_Pistol_Bullet"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CSkill_Pistol_Bullet::Free()
{
    Safe_Release(m_pOriginTransform);
    Safe_Release(m_pShaderCom);
    Safe_Release(m_pVIBufferCom);
    Safe_Release(m_pTextureCom);
    Safe_Release(m_pColliderCom);
    Safe_Release(m_pNavigationCom);

	__super::Free();
}
