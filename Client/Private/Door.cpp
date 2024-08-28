#include "stdafx.h"

#include "Door.h"
#include "Player.h"
#include "GameInstance.h"

CDoor::CDoor(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CElement{ pDevice , pContext }
{
}

CDoor::CDoor(const CDoor& pPrototype)
	:CElement{ pPrototype }
{
}

HRESULT CDoor::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CDoor::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	DECO_DESC* Desc = static_cast<DECO_DESC*>(pArg);
		
	m_pModelCom->SetUp_Animation(0, false);

	return S_OK;
}

void CDoor::Priority_Update(_float fTimeDelta)
{
	m_pGameInstance->Get_Layer(LEVEL_GAMEPLAY, TEXT("Layer_Monster")).empty() ? m_isEnable = true : m_isEnable = false;
	m_isEnable ? m_fAnimFrame = fTimeDelta : m_fAnimFrame = 0.f;
}

void CDoor::Update(_float fTimeDelta)
{
	m_pColliderCom->Update(m_pTransformCom->Get_WorldMatrix());
	m_pModelCom->Play_Animation(m_fAnimFrame);

	CCollider* pPlayerCollider = dynamic_cast<CCollider*>(m_pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_Player"), TEXT("Com_Collider")));
	CTransform* pPayerTransform = dynamic_cast<CTransform*>(m_pGameInstance->Get_Component(LEVEL_GAMEPLAY, TEXT("Layer_Player"), TEXT("Com_Transform")));

	if (m_pColliderCom->isCollision(pPlayerCollider))
	{
		if (!m_isEnable)
		{
			_vector vPlayerPos = pPayerTransform->Get_State(CTransform::STATE_POSITION);
			_vector vPos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
			_vector vDir = -XMVector3Normalize(m_pTransformCom->Get_State(CTransform::STATE_LOOK));

			_vector vMovePos = vPlayerPos + vDir * pPayerTransform->Get_Speed() * fTimeDelta;
			pPayerTransform->Set_State(CTransform::STATE_POSITION, XMVectorSetY(vMovePos, XMVectorGetY(vPlayerPos)));
		}
	}
	 
}

void CDoor::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);

	m_pGameInstance->Add_RenderObject(CRenderer::RG_NONBLEND, this);


#ifdef _DEBUG
	m_pGameInstance->Add_DebugComponent(m_pColliderCom);
#endif

}

HRESULT CDoor::Render()
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

HRESULT CDoor::Ready_Components()
{

	/* For.Com_Model */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Door"),
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxAnimMesh"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;


	
	/* For.Com_Collider */
	CBounding_OBB::BOUNDING_OBB_DESC	BoundingDesc{};

	_float fDegree =  XMConvertToDegrees(m_tFileData->fRotation);
	BoundingDesc.vExtents = _float3(0.5f, 4.5f, 5.f);
	BoundingDesc.vCenter = _float3(0.f, BoundingDesc.vExtents.y / 2.f - 2.f, 0.f);
	BoundingDesc.vRotation = _float3(0.f, fDegree - 5.f, 0.f);
	BoundingDesc.pObject = this;

	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_OBB"),
		TEXT("Com_Collider"), reinterpret_cast<CComponent**>(&m_pColliderCom), &BoundingDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CDoor::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;

	return S_OK;
}

CDoor* CDoor::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CDoor* pInstance = new CDoor(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CDoor"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CDoor::Clone(void* pArg)
{
	CDoor* pInstance = new CDoor(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CDoor"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CDoor::Free()
{
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pColliderCom);

	__super::Free();
}
