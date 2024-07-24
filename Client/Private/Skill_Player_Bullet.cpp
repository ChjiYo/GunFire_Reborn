#include "stdafx.h"

#include "Skill_Player_Bullet.h"
#include "GameInstance.h"   

CSkill_Player_Bullet::CSkill_Player_Bullet(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CSkill_Player { pDevice , pContext }
{
}

CSkill_Player_Bullet::CSkill_Player_Bullet(const CSkill& Prototype)
    :CSkill_Player{ Prototype }
{
}

HRESULT CSkill_Player_Bullet::Initialize_Prototype()
{
    if(FAILED(__super::Initialize_Prototype()))
        return E_FAIL;

	return S_OK;
}

HRESULT CSkill_Player_Bullet::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(Ready_Components()))
        return E_FAIL;

    SKILL_DESC* Desc = static_cast<SKILL_DESC*>(pArg);
    m_pTargetTransform = Desc->pTargetTransform;

    Safe_AddRef(m_pTargetTransform);

    m_pTransformCom->Set_State(CTransform::STATE_POSITION, m_pTargetTransform->Get_State(CTransform::STATE_POSITION));
    m_pTransformCom->Radian_Turn({0.f, 1.f, 0.f} , XMConvertToRadians(m_pTargetTransform->Look_Degree() + 80.f));
    m_vLook = m_pTargetTransform->Get_State(CTransform::STATE_LOOK);

    //m_pTransformCom->Set_State(CTransform::STATE_UP, m_pTargetTransform->Get_State(CTransform::STATE_UP));
    

    //네비게이션 메쉬 밖으로 나가면 delete? good 
    return S_OK;
}

void CSkill_Player_Bullet::Priority_Update(_float fTimeDelta)
{
}

void CSkill_Player_Bullet::Update(_float fTimeDelta)
{
   _vector	vPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
    vPosition += m_vLook * 5.f * fTimeDelta;
   
    m_pTransformCom->Set_State(CTransform::STATE_POSITION, vPosition);

    Destroy(fTimeDelta);
}

void CSkill_Player_Bullet::Late_Update(_float fTimeDelta)
{
    m_pGameInstance->Add_RenderObject(CRenderer::RG_BLEND, this);
}

HRESULT CSkill_Player_Bullet::Render()
{
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

HRESULT CSkill_Player_Bullet::Ready_Components()
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

    return S_OK;
}

HRESULT CSkill_Player_Bullet::Bind_ShaderResources()
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

void CSkill_Player_Bullet::Destroy(_float fTimeDelta)
{
    m_fTimer += fTimeDelta;

    if(m_fTimer > 5.f)
        m_bDead = true;
  
}

CSkill_Player_Bullet* CSkill_Player_Bullet::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CSkill_Player_Bullet* pInstance = new CSkill_Player_Bullet(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX(TEXT("Failed to Created : CSkill_Player_Bullet"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CSkill_Player_Bullet::Clone(void* pArg)
{
    CSkill_Player_Bullet* pInstance = new CSkill_Player_Bullet(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX(TEXT("Failed to Cloned : CSkill_Player_Bullet"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CSkill_Player_Bullet::Free()
{
    Safe_Release(m_pTargetTransform);
    Safe_Release(m_pShaderCom);
    Safe_Release(m_pVIBufferCom);
    Safe_Release(m_pTextureCom);

	__super::Free();
}
