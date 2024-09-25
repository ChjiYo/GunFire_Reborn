#include "stdafx.h"
#include "..\Public\Mon_Bug.h"

#include "GameInstance.h"

#include "FSM_Mon_Bug_Idle.h"
#include "FSM_Mon_Bug_Run.h"
#include "FSM_Mon_Bug_Attack.h"

#include "Player.h"

#include "VFX.h"

CMon_Bug::CMon_Bug(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CMonster{ pDevice, pContext }
{

}

CMon_Bug::CMon_Bug(const CMon_Bug& Prototype)
	: CMonster{ Prototype }
{

}

HRESULT CMon_Bug::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CMon_Bug::Initialize(void* pArg)
{
	CContainerObject::CONTAINEROBJECT_DESC		ContainerDesc{};
	ContainerDesc.fRotationPerSec = XMConvertToRadians(90.0f);
	ContainerDesc.fSpeedPerSec = 2.5f;
	ContainerDesc.iNumParts = PART_HEAD;

	if (FAILED(__super::Initialize(&ContainerDesc)))
		return E_FAIL;

	MON_DESC* MonDesc = static_cast<MON_DESC*>(pArg);
	m_tMonDesc.iMaxHp = MonDesc->iMaxHp;
	m_tMonDesc.iHp = MonDesc->iHp;
	m_tMonDesc.vPos = MonDesc->vPos;
	m_tMonDesc.iNum = MonDesc->iNum;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Ready_State()))
		return E_FAIL;

	if (FAILED(Ready_UI()))
		return E_FAIL;

	m_pStateCom->Set_State(TEXT("Monster_Bug_State_Idle"));

	switch (m_tMonDesc.iNum)
	{
	case 0:
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, {4.f, 2.f, 25.f, 1.f});
		break;

	case 1:
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, { 1.5f, 2.f, 25.f, 1.f });
		break;

	case 2:
		m_pTransformCom->Set_State(CTransform::STATE_POSITION, { 18.5f, 1.f, 17.5f, 1.f });
		break;

	}


	m_pNavigationCom->Init_Index(m_pTransformCom);

	return S_OK;
}

void CMon_Bug::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);

	m_pStateCom->Priority_Update(fTimeDelta);


	if (m_bNoise)
		m_fTimer += fTimeDelta;
}

void CMon_Bug::Update(_float fTimeDelta)
{
	if (!m_isMove)
		fTimeDelta = 0.f;

	__super::Update(fTimeDelta);

	m_pModelCom->Play_Animation(fTimeDelta);

	m_pColliderCom[COLL_AABB]->Update(m_pTransformCom->Get_WorldMatrix());
	
	Destroy();

	m_pStateCom->Update(fTimeDelta);
}

void CMon_Bug::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);

	m_pGameInstance->Add_RenderObject(CRenderer::RG_NONBLEND, this);

	m_pStateCom->Late_Update(fTimeDelta);

#ifdef _DEBUG
	m_pGameInstance->Add_DebugComponent(m_pColliderCom[COLL_AABB]);
	
#endif
}

HRESULT CMon_Bug::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	_uint		iNumMeshes = m_pModelCom->Get_NumMeshes();

	for (size_t i = 0; i < iNumMeshes; i++)
	{
		if (FAILED(m_pModelCom->Bind_MaterialSRV(m_pShaderCom, aiTextureType_DIFFUSE, "g_DiffuseTexture", 0)))
			return E_FAIL;

		m_pModelCom->Bind_BoneMatrices(m_pShaderCom, "g_BoneMatrices", i);

		if (FAILED(m_pShaderCom->Begin(5)))
			return E_FAIL;

		if (FAILED(m_pModelCom->Render(i)))
			return E_FAIL;
	}

	return S_OK;
}

void CMon_Bug::Destroy()
{
		if (m_tMonDesc.iHp <= 0)
		{
			m_pModelCom->SetUp_Animation(0, false);
			m_pTransformCom->Set_Speed(0);
			m_bNoise = true;

			if (!m_bDeadEffect)
			{
				CVFX::VFX_DESC Desc = {};
				Desc.vCreatePos = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
				m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_VFX_Destroy"), TEXT("Layer_Effect"), &Desc);
				m_bDeadEffect = true;
			}
		}
		if (m_tMonDesc.iHp <= 0 && m_fTimer >= 1)
		{
			DropItem();
			m_bDead = true;
		}
}

HRESULT CMon_Bug::Ready_State()
{
	CFSM_Monster::FSM_MON_DESC Desc = {};
	Desc.pTransform = m_pTransformCom;
	Desc.pNavigation = m_pNavigationCom;
	Desc.pModel = m_pModelCom;

	m_pStateCom->Add_State(CFSM_Mon_Bug_Idle::Create(m_pDevice, m_pContext, Desc), TEXT("Monster_Bug_State_Idle"));
	m_pStateCom->Add_State(CFSM_Mon_Bug_Run::Create(m_pDevice, m_pContext, Desc), TEXT("Monster_Bug_State_Run"));
	m_pStateCom->Add_State(CFSM_Mon_Bug_Attack::Create(m_pDevice, m_pContext, Desc), TEXT("Monster_Bug_State_Attack"));

	return S_OK;
}

HRESULT CMon_Bug::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	/* Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxAnimMesh"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* Com_Model */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Monster_Bug"),
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;


	/* Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Noise"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	/* Com_State */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_State"),
		TEXT("Com_State"), reinterpret_cast<CComponent**>(&m_pStateCom))))
		return E_FAIL; 

	/* For.Com_Collider */
	CBounding_AABB::BOUNDING_AABB_DESC	BoundingDesc{};

	BoundingDesc.vExtents = _float3(0.5f, 0.5f, 0.5f);
	BoundingDesc.vCenter = _float3(0.f, BoundingDesc.vExtents.y, 0.f);
	BoundingDesc.pObject = this;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Collider_AABB"),
		TEXT("Com_ColliderAABB"), reinterpret_cast<CComponent**>(&m_pColliderCom[COLL_AABB]), &BoundingDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CMon_Bug::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_ShaderResources(m_pShaderCom, "g_NoiseTexture")))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_Timer", &m_fTimer, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_vCamPosition", &m_pGameInstance->Get_CamPosition_Float4(), sizeof(_float4))))
		return E_FAIL;

	return S_OK;
}

CMon_Bug* CMon_Bug::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CMon_Bug* pInstance = new CMon_Bug(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CMon_Bug"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CMon_Bug::Clone(void* pArg)
{
	CMon_Bug* pInstance = new CMon_Bug(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CMon_Bug"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMon_Bug::Free()
{
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pStateCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pNavigationCom);
	Safe_Release(m_pTextureCom);

	for (size_t i = 0; i < COLL_END; i++)
		Safe_Release(m_pColliderCom[i]);

	__super::Free();
}
