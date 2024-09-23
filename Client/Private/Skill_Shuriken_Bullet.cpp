#include "stdafx.h"

#include "Skill_Shuriken_Bullet.h"
#include "GameInstance.h"   
#include "Cell.h"

#include "Wall_101.h"
#include "Wall_102.h"

#include "VFX.h"

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

    m_pNaviCom->Init_Index(m_pTransformCom);

    return S_OK;
}

void CSkill_Shuriken_Bullet::Priority_Update(_float fTimeDelta)
{
    m_fDestroyTimer += fTimeDelta;
    m_fReCollTimer += fTimeDelta;
}

void CSkill_Shuriken_Bullet::Update(_float fTimeDelta)
{
    if (m_fReCollTimer > 0.25f)
    {
        m_fReCollTimer = 0.f;
        m_bWallColl = false;
    }

    m_pColliderCom->Update(m_pTransformCom->Get_WorldMatrix());

    _vector	vPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

    vPosition += m_vTargetLook * 35.f * fTimeDelta;

    m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPosition);

    NaviReflect();
    HighReflect();

    Destroy(fTimeDelta, vPosition);
}

void CSkill_Shuriken_Bullet::Late_Update(_float fTimeDelta)
{
    __super::Late_Update(fTimeDelta);

    list<CGameObject*> vecWall = m_pGameInstance->Get_Layer(LEVEL_GAMEPLAY, TEXT("Layer_Wall_101"));
    list<CGameObject*> vecWall2 = m_pGameInstance->Get_Layer(LEVEL_GAMEPLAY, TEXT("Layer_Wall_102"));

    for (auto& iter : vecWall)
    {
        CWall_101* pWall = dynamic_cast<CWall_101*>(iter);

        if (m_pColliderCom->isCollision(pWall->Get_Collider()))
        {

            _vector vBulletLook = m_pTransformCom->Get_State(CTransform::STATE_LOOK);
            _vector vWallLook = pWall->Get_Transform()->Get_State(CTransform::STATE_LOOK);

            if (!m_bWallColl)
            {
                m_vTargetLook = XMVector3Normalize(XMVector3Reflect(m_vTargetLook, vWallLook));
                m_bWallColl = true;
                m_iNumBounding++;

                _vector vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
                m_pTransformCom->LookAt(vPos + m_vTargetLook * 2.f);
                m_pTransformCom->Radian_Turn(m_pTransformCom->Get_State(CTransform::STATE_UP), XMConvertToRadians(90.f));

                CVFX::VFX_DESC Desc = {};
                Desc.vCreatePos = vPos;

                m_pGameInstance->Add_GameObject_ToLayer(m_pGameInstance->GetCurrentLevelIndex(), TEXT("Prototype_GameObject_VFX_Shuriken"), TEXT("Layer_Effect"), &Desc);
            }
        }
    }

    for (auto& iter : vecWall2)
    {
        CWall_102* pWall2 = dynamic_cast<CWall_102*>(iter);

        if (m_pColliderCom->isCollision(pWall2->Get_Collider()))
        {

            _vector vBulletLook = m_pTransformCom->Get_State(CTransform::STATE_LOOK);
            _vector vWallLook = pWall2->Get_Transform()->Get_State(CTransform::STATE_LOOK);

            if (!m_bWallColl)
            {
                m_vTargetLook = XMVector3Normalize(XMVector3Reflect(m_vTargetLook, vWallLook));
                m_bWallColl = true;
                m_iNumBounding++;

                _vector vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
                m_pTransformCom->LookAt(vPos + m_vTargetLook * 2.f);
                m_pTransformCom->Radian_Turn(m_pTransformCom->Get_State(CTransform::STATE_UP), XMConvertToRadians(90.f));

                CVFX::VFX_DESC Desc = {};
                Desc.vCreatePos = vPos;

                m_pGameInstance->Add_GameObject_ToLayer(m_pGameInstance->GetCurrentLevelIndex(), TEXT("Prototype_GameObject_VFX_Shuriken"), TEXT("Layer_Effect"), &Desc);
            }
        }
    }

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
    CNavigation::NAVIGATION_DESC		NavigationDesc{};
    NavigationDesc.iCurrentCellIndex = 0;

    /* Com_Navigation */
    if (FAILED(__super::Add_Component(m_pGameInstance->GetLoadingLevelIndex(), TEXT("Prototype_Component_Navigation"),
        TEXT("Com_Navigation"), reinterpret_cast<CComponent**>(&m_pNaviCom), &NavigationDesc)))
        return E_FAIL;

    /* For.Com_Shader */
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxPosTex"),
        TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
        return E_FAIL;

    /* For.Com_Model */
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Skill_Bullet_Red"),
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

void CSkill_Shuriken_Bullet::Destroy(_float fTimeDelta, _vector vPos)
{
    m_fDestroyTimer += fTimeDelta;

    if (m_fDestroyTimer > 15.f)
        m_bDead = true;

    if(m_iNumBounding >= 4)
        m_bDead = true;
}

void CSkill_Shuriken_Bullet::NaviReflect()
{
    _uint iCurrNavi =  m_pNaviCom->Init_Index(m_pTransformCom);
    _float3 vCellPos[CCell::POINT_END] = {};
    _vector vCellPosPtr[CCell::POINT_END] = {};

    _float fHeight = m_pNaviCom->Navi_Height(m_pTransformCom);
    _float vPosY = XMVectorGetY(m_pTransformCom->Get_State(CTransform::STATE_POSITION));

    _vector vNormal = {};

    for (int i = 0; i < CCell::POINT_END; ++i)
    {
        vCellPos[i] = m_pNaviCom->Get_Cell().at(iCurrNavi)->Get_Point(i);
        vCellPosPtr[i] = XMLoadFloat3(&vCellPos[i]);

        _vector V1 = XMVectorSubtract(vCellPosPtr[1] , vCellPosPtr[0]);
        _vector V2 = XMVectorSubtract(vCellPosPtr[2], vCellPosPtr[0]);

        m_vReflectVec = XMVector3Normalize(XMVector3Cross(V1, V2));
    }
    

    if (vPosY <= fHeight + 0.5f && vPosY >= fHeight - 0.5f)
    {
        if (!m_bWallColl)
        {
            m_vTargetLook = XMVector3Normalize(XMVector3Reflect(m_vTargetLook, m_vReflectVec));
            m_bWallColl = true;
            m_iNumBounding++;

            _vector vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
            m_pTransformCom->LookAt(vPos  + m_vTargetLook * 2.f);
            m_pTransformCom->Radian_Turn(m_pTransformCom->Get_State(CTransform::STATE_UP), XMConvertToRadians(90.f));

            CVFX::VFX_DESC Desc = {};
            Desc.vCreatePos = vPos;

            m_pGameInstance->Add_GameObject_ToLayer(m_pGameInstance->GetCurrentLevelIndex(), TEXT("Prototype_GameObject_VFX_Shuriken"), TEXT("Layer_Effect"), &Desc);
        }
    }



}

void CSkill_Shuriken_Bullet::HighReflect()
{
    _float vPosY = XMVectorGetY(m_pTransformCom->Get_State(CTransform::STATE_POSITION));

    if (vPosY > 6.5f)
    {
        if (!m_bWallColl)
        {
            m_vTargetLook = XMVector3Normalize(XMVector3Reflect(m_vTargetLook, {0, -1, 0}));
            m_bWallColl = true;
            m_iNumBounding++;

            _vector vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
            m_pTransformCom->LookAt(vPos + m_vTargetLook * 2.f);
            m_pTransformCom->Radian_Turn(m_pTransformCom->Get_State(CTransform::STATE_UP), XMConvertToRadians(90.f));

            CVFX::VFX_DESC Desc = {};
            Desc.vCreatePos = vPos;

            m_pGameInstance->Add_GameObject_ToLayer(m_pGameInstance->GetCurrentLevelIndex(), TEXT("Prototype_GameObject_VFX_Shuriken"), TEXT("Layer_Effect"), &Desc);
        }
    }

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
    Safe_Release(m_pNaviCom);

    __super::Free();
}
