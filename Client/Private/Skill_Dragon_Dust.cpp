#include "stdafx.h"

#include "Skill_Dragon_Dust.h"

#include "GameInstance.h"

#include "Player.h"
#include "VFX_Mon_Dust.h"

CSkill_Dragon_Dust::CSkill_Dragon_Dust(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CSkill_Monster{ pDevice , pContext }
{
}

CSkill_Dragon_Dust::CSkill_Dragon_Dust(const CSkill_Dragon_Dust& Prototype)
    :CSkill_Monster{ Prototype }
{
}

HRESULT CSkill_Dragon_Dust::Initialize_Prototype()
{
    if (FAILED(__super::Initialize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CSkill_Dragon_Dust::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(Ready_Components()))
        return E_FAIL;

    if (FAILED(Ready_PartObjects()))
        return E_FAIL;

    Safe_AddRef(m_pOriginTransform);

    m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSetY(m_pOriginTransform->Get_State(CTransform::STATE_POSITION) , 0.f));

    m_bAttack = false;

    return S_OK;
}

void CSkill_Dragon_Dust::Priority_Update(_float fTimeDelta)
{
    __super::Priority_Update(fTimeDelta);
    m_fDistance += fTimeDelta *10.f;
    m_fDestroyTimer += fTimeDelta;
}

void CSkill_Dragon_Dust::Update(_float fTimeDelta)
{
    __super::Update(fTimeDelta);
    Destroy(fTimeDelta);
    m_pColliderCom->Update(m_pTransformCom->Get_WorldMatrix());
}

void CSkill_Dragon_Dust::Late_Update(_float fTimeDelta)
{
    __super::Late_Update(fTimeDelta);

    _vector vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

    _uint iNum = m_pGameInstance->GetLoadingLevelIndex();
    CPlayer* pPlayer = dynamic_cast<CPlayer*>(m_pGameInstance->Get_Layer(iNum, TEXT("Layer_Player")).back());
    _vector vPlayerPos =  pPlayer->Get_Transform()->Get_State(CTransform::STATE_POSITION);
  
    if(IsCollider(vPos, vPlayerPos , m_fDistance) && XMVectorGetY(vPlayerPos) <= 1.5f)
    {
        pPlayer->Set_Damage(-5);
        m_fDistance = 0.f;
        //m_bAttack = true;
    }
    
#ifdef _DEBUG
    m_pGameInstance->Add_DebugComponent(m_pColliderCom);
#endif

    m_pGameInstance->Add_RenderObject(CRenderer::RG_BLEND, this);
}

HRESULT CSkill_Dragon_Dust::Render()
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

HRESULT CSkill_Dragon_Dust::Ready_Components()
{
    /* For.Com_Shader */
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxPosTex"),
        TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
        return E_FAIL;

    /* For.Com_Model */
    if (FAILED(__super::Add_Component(LEVEL_BOSS, TEXT("Prototype_Component_Texture_Skill_Dragon_Bubble"),
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

HRESULT CSkill_Dragon_Dust::Ready_PartObjects()
{
    /* 바디객체를 추가하자 */
    CPartObject::PARTOBJECT_DESC			BodyDesc{};
    BodyDesc.pParentMatrix = m_pTransformCom->Get_WorldMatrixPtr();

    if (FAILED(__super::Add_PartObject(PART_EFFECT, TEXT("Prototype_GameObject_VFX_Dragon_Dust"), &BodyDesc)))
        return E_FAIL;

    return S_OK;
}

HRESULT CSkill_Dragon_Dust::Bind_ShaderResources()
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

void CSkill_Dragon_Dust::Destroy(_float fTimeDelta)
{
    if (m_fDestroyTimer > 2.f)
    {
        m_bDead = true;
    }
}

_bool CSkill_Dragon_Dust::IsCollider(_vector vCollStartPos, _vector vCollObjPos, _float fDamageDist)
{
    _float vDistance =  XMVectorGetX(XMVector3Length(vCollObjPos - vCollStartPos));
    _float fRange = 1.5f;

    if (fabs(vDistance) < fDamageDist && fabs(vDistance) >= fDamageDist - fRange)
    {
        return true;
    }


    return false;
}

CSkill_Dragon_Dust* CSkill_Dragon_Dust::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CSkill_Dragon_Dust* pInstance = new CSkill_Dragon_Dust(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX(TEXT("Failed to Created : CSkill_Dragon_Dust"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CSkill_Dragon_Dust::Clone(void* pArg)
{
    CSkill_Dragon_Dust* pInstance = new CSkill_Dragon_Dust(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX(TEXT("Failed to Cloned : CSkill_Dragon_Dust"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CSkill_Dragon_Dust::Free()
{
    Safe_Release(m_pOriginTransform);
    Safe_Release(m_pShaderCom);
    Safe_Release(m_pVIBufferCom);
    Safe_Release(m_pTextureCom);
    Safe_Release(m_pColliderCom);

    __super::Free();
}

