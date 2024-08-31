#include "stdafx.h"

#include "VFX_Mon_Breath.h"
#include "GameInstance.h"

CVFX_Mon_Breath::CVFX_Mon_Breath(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CVFX_Monster{ pDevice ,pContext }
{
}

CVFX_Mon_Breath::CVFX_Mon_Breath(const CVFX_Mon_Breath& Prototype)
	:CVFX_Monster{ Prototype }
{
}

HRESULT CVFX_Mon_Breath::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CVFX_Mon_Breath::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

    if (FAILED(Ready_Components()))
        return E_FAIL;

    m_pTransformCom->Set_Scaled(1.f, 5.f,1.f);
    m_pTransformCom->Rotation({ 1.f, 0.f, 0.f }, XMConvertToRadians(90.f));
    m_pTransformCom->Radian_Turn(m_pTransformCom->Get_State(CTransform::STATE_UP), XMConvertToRadians(90.f));
    m_pTransformCom->Set_State(CTransform::STATE_POSITION, { 0.f,2.f, 8.f, 1.f });
	return S_OK;
}

void CVFX_Mon_Breath::Priority_Update(_float fTimeDelta)
{
    m_fTimer += fTimeDelta;
    if (m_fTimer >= 1.f)
        m_fTimer = 0.f;

    __super::Priority_Update(fTimeDelta);
}

void CVFX_Mon_Breath::Update(_float fTimeDelta)
{
    __super::Update(fTimeDelta);
}

void CVFX_Mon_Breath::Late_Update(_float fTimeDelta)
{
    __super::Late_Update(fTimeDelta);

    m_pGameInstance->Add_RenderObject(CRenderer::RG_BLEND, this);
}

HRESULT CVFX_Mon_Breath::Render()
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

HRESULT CVFX_Mon_Breath::Ready_Components()
{ /* For.Com_Shader */
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_Effect_MaskTex"),
        TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
        return E_FAIL;

    /* For.Com_Model */
    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_VFX_Dragon_Breath"),
        TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
        return E_FAIL;

    /* For.Com_VIBuffer */
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"),
        TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
        return E_FAIL;

	return S_OK;
}

HRESULT CVFX_Mon_Breath::Bind_ShaderResources()
{
    if (FAILED(__super::Bind_WorldMatrix(m_pShaderCom, "g_WorldMatrix")))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
        return E_FAIL;

     if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_Texture", 0)))
         return E_FAIL;

     if (FAILED(m_pShaderCom->Bind_RawValue("g_Timer", &m_fTimer, sizeof(_float))))
         return E_FAIL;

    return S_OK;
}

CVFX_Mon_Breath* CVFX_Mon_Breath::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CVFX_Mon_Breath* pInstance = new CVFX_Mon_Breath(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX(TEXT("Failed to Created : CVFX_Mon_Breath"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CVFX_Mon_Breath::Clone(void* pArg)
{
    CVFX_Mon_Breath* pInstance = new CVFX_Mon_Breath(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX(TEXT("Failed to Cloned : CVFX_Mon_Breath"));
        Safe_Release(pInstance);
    }

    return pInstance;
}


void CVFX_Mon_Breath::Free()
{
    Safe_Release(m_pShaderCom);
    Safe_Release(m_pTextureCom);
    Safe_Release(m_pVIBufferCom);

	__super::Free();
}
