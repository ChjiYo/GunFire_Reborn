#include "stdafx.h"

#include "UI_Number.h"
#include "GameInstance.h"

CUI_Number::CUI_Number(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CUIObject{ pDevice ,pContext }
{
}

CUI_Number::CUI_Number(const CUI_Number& Prototype)
    :CUIObject{ Prototype }
{
}

HRESULT CUI_Number::Initialize_Prototype()
{
    if (FAILED(__super::Initialize_Prototype()))
        return E_FAIL;


    return S_OK;
}

HRESULT CUI_Number::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    UI_NUM_DESC* Desc = static_cast<UI_NUM_DESC*>(pArg);
    m_iTextureIndex = Desc->iTextureIndex;
    m_iNumTexture = Desc->iNumUI;
    m_iAngle = Desc->iAngle;
    _uint iNumCnt = Desc->iNumCnt;

    m_pOriginTransform = Desc->pTransform;
    Safe_AddRef(m_pOriginTransform);

    if (FAILED(Ready_Component()))
        return E_FAIL;

   // _vector vPos = m_pOriginTransform->Get_State(CTransform::STATE_POSITION) + (m_pOriginTransform->Get_State(CTransform::STATE_RIGHT) * ((Desc->iNumCnt) * 0.5f)) + (m_pOriginTransform->Get_State(CTransform::STATE_LOOK) * ((Desc->iNumCnt) * -0.5f));
    //m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);
    if (!m_pGameInstance->Get_Layer(m_pGameInstance->GetLoadingLevelIndex(), TEXT("Layer_UI_Number")).empty())
    {
        _matrix vMatPivot = m_pGameInstance->Get_Layer(m_pGameInstance->GetLoadingLevelIndex(), TEXT("Layer_UI_Number")).front()->Get_Transform()->Get_WorldMatrix();
        // vMatPivot.r[CTransform::STATE_POSITION] += vMatPivot.r[CTransform::STATE_LOOK] * 1.25f * (iNumCnt + 1);
        m_pTransformCom->Set_WorldMatrix(vMatPivot);
    }
    else
        m_pTransformCom->Set_State(CTransform::STATE_POSITION , m_pOriginTransform->Get_State(CTransform::STATE_POSITION));
    
    m_pTransformCom->Set_Scaled(0.5f , 0.5f, 0.5f);

    return S_OK;
}

void CUI_Number::Priority_Update(_float fTimeDelta)
{
    CTransform* pPlayerTransform = dynamic_cast<CTransform*>(m_pGameInstance->Get_Component(m_pGameInstance->GetCurrentLevelIndex(), TEXT("Layer_Player"), TEXT("Com_Transform")));
    m_pTransformCom->LookAt(pPlayerTransform->Get_State(CTransform::STATE_POSITION));
    m_pTransformCom->Radian_Turn(m_pTransformCom->Get_State(CTransform::STATE_UP), XMConvertToRadians(180.f));

    if(!m_pTransformCom->Gravity(0.2f, XMVectorGetY(m_pOriginTransform->Get_State(CTransform::STATE_POSITION)), fTimeDelta))
        m_pTransformCom->Go_Up(fTimeDelta);
    
    //나중에 그냥 쉐이더로 던지자 그게 마음 편하다 진짜로
    m_fDeleteTimer += fTimeDelta;
}

void CUI_Number::Update(_float fTimeDelta)
{
  

    _float WarfPosX = XMVectorGetX(m_pOriginTransform->Get_State(CTransform::STATE_POSITION)) * cos(m_iAngle * (3.14f / 180.f));
    _float WarfPosZ = XMVectorGetZ(m_pOriginTransform->Get_State(CTransform::STATE_POSITION)) * sin(m_iAngle * (3.14f / 180.f));

    _vector vDir = { WarfPosX  ,XMVectorGetY(m_pOriginTransform->Get_State(CTransform::STATE_POSITION)) , WarfPosZ , 1.f};
    vDir = XMVector3Normalize(vDir);

    _vector vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
    vPos += vDir * 2.f * fTimeDelta;
    m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPos);

    Destroy(fTimeDelta);
}

void CUI_Number::Late_Update(_float fTimeDelta)
{
    m_pGameInstance->Add_RenderObject(CRenderer::RG_UI, this);
}

HRESULT CUI_Number::Render()
{
    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;

    if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", m_iTextureIndex)))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Begin(4)))
        return E_FAIL;

    if (FAILED(m_pVIBufferCom->Bind_Buffers()))
        return E_FAIL;

    if (FAILED(m_pVIBufferCom->Render()))
        return E_FAIL;

    return S_OK;
}

HRESULT CUI_Number::Ready_Component()
{
    //if (FAILED(__super::Ready_Components()))
    //    return E_FAIL;

    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_UI_VtxRect"),
        TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
        return E_FAIL;

    /* For.Com_VIBuffer */
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"),
        TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
        return E_FAIL;

    /* For.Com_Texture */
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Number"),
        TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
        return E_FAIL;

    return S_OK;
}

HRESULT CUI_Number::Bind_ShaderResources()
{
    //if (FAILED(__super::Bind_Perspective()))
    //    return E_FAIL;

    if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_RawValue("g_Timer" , &m_fDeleteTimer, sizeof(_float))))
        return E_FAIL;

    return S_OK;
}

void CUI_Number::Destroy(_float fTimeDelta)
{
    if (m_fDeleteTimer >= 1.f)
    {
        m_bDead = true;
        m_fDeleteTimer = 0.f;
    }
}

CUI_Number* CUI_Number::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CUI_Number* pInstance = new CUI_Number(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX(TEXT("Failed to Created : CUI_Cross_Hair"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CUI_Number::Clone(void* pArg)
{
    CUI_Number* pInstance = new CUI_Number(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX(TEXT("Failed to Cloned : CUI_Cross_Hair"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CUI_Number::Free()
{
    Safe_Release(m_pOriginTransform);

    __super::Free();
}
