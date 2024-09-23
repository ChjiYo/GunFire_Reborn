#include "stdafx.h"

#include "Shield.h"
#include "GameInstance.h"

CShield::CShield(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CPartObject{ pDevice  , pContext }
{
}

CShield::CShield(const CShield& Prototype)
    :CPartObject{ Prototype }
{
}

HRESULT CShield::Initialize_Prototype()
{
    if (FAILED(__super::Initialize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CShield::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    WEAPON_DESC* pDesc = static_cast<WEAPON_DESC*>(pArg);

    m_pParentState = pDesc->pParentState;
    m_pSocketMatrix = pDesc->pSocketBoneMatrix;

    if (FAILED(Ready_Components()))
        return E_FAIL;

    m_pTransformCom->Set_Scaled(0.1f, 0.1f, 0.1f);
    m_pTransformCom->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(90.f));

    m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(0.0f, -1.5f, 0.f, 1.f));
    

    return S_OK;
}

void CShield::Priority_Update(_float fTimeDelta)
{
    if (fTimeDelta == 0.f)
        return;

    __super::Priority_Update(fTimeDelta);
}

void CShield::Update(_float fTimeDelta)
{
    __super::Update(fTimeDelta);

    _matrix		SocketBoneMatrix = XMLoadFloat4x4(m_pSocketMatrix);

    for (size_t i = 0; i < 3; i++)
        SocketBoneMatrix.r[i] = XMVector3Normalize(SocketBoneMatrix.r[i]);


    XMStoreFloat4x4(&m_WorldMatrix, m_pTransformCom->Get_WorldMatrix() * SocketBoneMatrix * XMLoadFloat4x4(m_pParentMatrix));


    m_pColliderCom->Update(XMLoadFloat4x4(&m_WorldMatrix));
}

void CShield::Late_Update(_float fTimeDelta)
{
    __super::Late_Update(fTimeDelta);

    m_pGameInstance->Add_RenderObject(CRenderer::RG_NONBLEND, this);

#ifdef _DEBUG
    
    m_pGameInstance->Add_DebugComponent(m_pColliderCom);
    
#endif
}

HRESULT CShield::Render()
{
    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;

    return S_OK;
}

HRESULT CShield::Ready_Components()
{
    /* For.Com_Shader */
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxMesh"),
        TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
        return E_FAIL;

    /* For.Com_Collider */
    CBounding_OBB::BOUNDING_OBB_DESC	AABBBoundingDesc{};

    AABBBoundingDesc.vRotation = _float3(45.f, 0.f, 0.f);
    AABBBoundingDesc.vExtents = _float3(15.f, 10.f, 5.f);
    AABBBoundingDesc.vCenter = _float3( 0.f, AABBBoundingDesc.vExtents.y , 0.f);
    AABBBoundingDesc.pObject = this;

    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_OBB"),
        TEXT("Com_ColliderAABB"), reinterpret_cast<CComponent**>(&m_pColliderCom), &AABBBoundingDesc)))
        return E_FAIL;

    return S_OK;
}

HRESULT CShield::Bind_ShaderResources()
{
    if (FAILED(__super::Bind_WorldMatrix(m_pShaderCom, "g_WorldMatrix")))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
        return E_FAIL;

    return S_OK;
}


CShield* CShield::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CShield* pInstance = new CShield(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX(TEXT("Failed to Created : CShield"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CShield::Clone(void* pArg)
{
    CShield* pInstance = new CShield(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX(TEXT("Failed to Cloned : CShield"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CShield::Free()
{
    Safe_Release(m_pShaderCom);
    Safe_Release(m_pColliderCom);

    __super::Free();
}
