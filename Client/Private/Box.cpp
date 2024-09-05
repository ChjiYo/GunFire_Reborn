#include "stdafx.h"

#include "Box.h"
#include "Player.h"
#include "Item.h"
#include "GameInstance.h"
#include "Animation.h"

CBox::CBox(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CElement{ pDevice , pContext }
{
}

CBox::CBox(const CBox& pPrototype)
	:CElement{ pPrototype }
{
}

HRESULT CBox::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CBox::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	return S_OK;
}

void CBox::Priority_Update(_float fTimeDelta)
{

}

void CBox::Update(_float fTimeDelta)
{
	m_pColliderCom->Update(m_pTransformCom->Get_WorldMatrix());
	m_pModelCom->Play_Animation(fTimeDelta);

}

void CBox::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);

	CCollider* pCollider = dynamic_cast<CCollider*>(m_pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_Player"), TEXT("Com_Collider")));

	if (m_bAnimStop && m_pColliderCom->isCollision(pCollider) && m_pGameInstance->Get_DIKeyState(DIK_F))
	{
		m_pModelCom->SetUp_Animation(1, false);
		m_bItemCreate = true;
		m_bAnimStop = false;
	}

	if (m_bItemCreate)
	{
		m_fItemCreateTimer += fTimeDelta;
		
		if (m_pModelCom->Get_Anim(1)->Get_Duration() - 0.3f <= m_fItemCreateTimer)
		{ 
			m_bItemCreate = false;
			CItem::ITEM_DESC Desc = {};
			Desc.pOriginTransform = m_pTransformCom;
			Desc.fSpeedPerSec = 3.f;

			m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Item_BoomGun"), TEXT("Layer_Item"), &Desc);
		}

	}

	m_pGameInstance->Add_RenderObject(CRenderer::RG_NONBLEND, this);


#ifdef _DEBUG
	m_pGameInstance->Add_DebugComponent(m_pColliderCom);
#endif

}

HRESULT CBox::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	_uint	iNumMeshes = m_pModelCom->Get_NumMeshes();

	for (size_t i = 0; i < iNumMeshes; i++)
	{
		if (FAILED(m_pModelCom->Bind_MaterialSRV(m_pShaderCom, aiTextureType_DIFFUSE, "g_DiffuseTexture", i)))
			return E_FAIL;

		m_pModelCom->Bind_BoneMatrices(m_pShaderCom, "g_BoneMatrices", i);

		if (FAILED(m_pShaderCom->Begin(1)))
			return E_FAIL;

		if (FAILED(m_pModelCom->Render(i)))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CBox::Ready_Components()
{

	/* For.Com_Model */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Box"),
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxAnimMesh"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* For.Com_Collider */
	CBounding_Sphere::BOUNDING_SPHERE_DESC	BoundingDesc{};

	BoundingDesc.fRadius = 2.5f;
	BoundingDesc.vCenter = _float3(0.f, BoundingDesc.fRadius / 2.f , 0.f);
	BoundingDesc.pObject = this;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_Sphere"),
		TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pColliderCom), &BoundingDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CBox::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;

	return S_OK;
}

CBox* CBox::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CBox* pInstance = new CBox(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CBox"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CBox::Clone(void* pArg)
{
	CBox* pInstance = new CBox(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CBox"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBox::Free()
{
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pColliderCom);

	__super::Free();
}
