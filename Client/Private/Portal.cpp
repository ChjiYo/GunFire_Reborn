#include "stdafx.h"

#include "Portal.h"
#include "Player.h"
#include "GameInstance.h"

#include "Wall_Stage1.h"
#include "Wall_Stage2.h"

#include "Level_Loading.h"

CPortal::CPortal(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CElement{ pDevice , pContext }
{
}

CPortal::CPortal(const CPortal& pPrototype)
	:CElement{ pPrototype }
{
}

HRESULT CPortal::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CPortal::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_pTransformCom->Set_Scaled(3.f, 10.f, 1.f);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, { 40.f, 10.f * 0.5f, 65.f, 1.f });
	m_pTransformCom->Rotation({ 0.f , 1.f ,0.f }, XMConvertToRadians(90.f));

	return S_OK;
}

void CPortal::Priority_Update(_float fTimeDelta)
{
	m_fTimer += fTimeDelta;

	if (m_fTimer >= 25.f)
		m_fTimer = 0.f;
}

void CPortal::Update(_float fTimeDelta)
{
	m_pColliderCom->Update(m_pTransformCom->Get_WorldMatrix());

}

void CPortal::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);

	m_pGameInstance->Add_RenderObject(CRenderer::RG_NONBLEND, this);

#ifdef _DEBUG
	m_pGameInstance->Add_DebugComponent(m_pColliderCom);
#endif
}

HRESULT CPortal::Render()
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

HRESULT CPortal::Ready_Components()
{
	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_STATIC , TEXT("Prototype_Component_Shader_Effect_MaskTex"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* For.Com_Model */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Portal"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	/* For.Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	CBounding_AABB::BOUNDING_AABB_DESC	BoundingDesc{};

	BoundingDesc.vExtents = _float3(1.0f, 1.0f, 1.0f);
	BoundingDesc.vCenter = _float3(0.f, BoundingDesc.vExtents.y * 0.5f, 0.f);
	BoundingDesc.pObject = this;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_AABB"),
		TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pColliderCom), &BoundingDesc)))
		return E_FAIL;


	return S_OK;
}

HRESULT CPortal::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
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

CPortal* CPortal::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CPortal* pInstance = new CPortal(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CPortal"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CPortal::Clone(void* pArg)
{
	CPortal* pInstance = new CPortal(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CPortal"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPortal::Free()
{
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pColliderCom);

	__super::Free();
}
