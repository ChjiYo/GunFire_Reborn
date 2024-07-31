#include "stdafx.h"
#include "..\Public\Mon_Archer.h"

#include "GameInstance.h"

#include "FSM_Mon_Archer_Idle.h"
#include "FSM_Mon_Archer_Hit.h"
#include "FSM_Mon_Archer_Run.h"
#include "FSM_Mon_Archer_Walk.h"
#include "FSM_Mon_Archer_Attack.h"

CMon_Archer::CMon_Archer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CMonster{ pDevice, pContext }
{

}

CMon_Archer::CMon_Archer(const CMon_Archer& Prototype)
	: CMonster{ Prototype }
{

}

HRESULT CMon_Archer::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CMon_Archer::Initialize(void* pArg)
{
	CContainerObject::CONTAINEROBJECT_DESC		ContainerDesc{};
	ContainerDesc.fRotationPerSec = XMConvertToRadians(90.0f);
	ContainerDesc.fSpeedPerSec = 2.5f;
	ContainerDesc.iNumParts = PART_HEAD;

	if (FAILED(__super::Initialize(&ContainerDesc)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if(FAILED(Ready_State()))
		return E_FAIL;

	m_pStateCom->Set_State(TEXT("Monster_Archer_State_Idle"));

	m_pModelCom->SetUp_Animation(1, true);
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(20.f, 1.f, 20.f, 1.f));

	return S_OK;
}

void CMon_Archer::Priority_Update(_float fTimeDelta)
{
	m_pStateCom->Priority_Update(fTimeDelta);
}

void CMon_Archer::Update(_float fTimeDelta)
{
	for(int i = 0 ; i < COLL_END ; ++i)
		m_pColliderCom[i]->Update(m_pTransformCom->Get_WorldMatrix());

	if (m_pGameInstance->Key_Down('N'))
	{
		m_iAnimFrame--;
		m_pModelCom->SetUp_Animation(m_iAnimFrame, false);
	}
	if (m_pGameInstance->Key_Down('M'))
	{
		m_iAnimFrame++;
		m_pModelCom->SetUp_Animation(m_iAnimFrame, false);
	}
	m_pStateCom->Update(fTimeDelta);
}

void CMon_Archer::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
	m_pModelCom->Play_Animation(fTimeDelta);
	m_pGameInstance->Add_RenderObject(CRenderer::RG_NONBLEND, this);

	m_pStateCom->Late_Update(fTimeDelta);
}

HRESULT CMon_Archer::Render()
{
#ifdef _DEBUG
	for(int i = 0 ; i < COLL_END ; ++i)
		m_pColliderCom[i]->Render();
#endif

	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	_uint		iNumMeshes = m_pModelCom->Get_NumMeshes();

	for (size_t i = 0; i < iNumMeshes; i++)
	{
		if (FAILED(m_pModelCom->Bind_MaterialSRV(m_pShaderCom, aiTextureType_DIFFUSE, "g_DiffuseTexture", i)))
			return E_FAIL;

		m_pModelCom->Bind_BoneMatrices(m_pShaderCom, "g_BoneMatrices", i);

		if (FAILED(m_pShaderCom->Begin(0)))
			return E_FAIL;

		if (FAILED(m_pModelCom->Render(i)))
			return E_FAIL;
	}

	wstring m_strMaxHp = to_wstring(m_iAnimFrame);
	m_pGameInstance->Draw_Font(TEXT("Font_Nexon"), m_strMaxHp, _float2(1200.f, 100.f), XMVectorSet(0.7f, 1.f, 0.3f, 1.f), XMConvertToRadians(-5.f), 0.8f);

	return S_OK;
}

HRESULT CMon_Archer::Ready_State()
{
	CFSM_Monster::FSM_MON_DESC Desc = {};
	Desc.pTransform = m_pTransformCom;
	Desc.pNavigation = m_pNavigationCom;
	Desc.pModel = m_pModelCom;

	m_pStateCom->Add_State(CFSM_Mon_Archer_Idle::Create(m_pDevice, m_pContext, Desc), TEXT("Monster_Archer_State_Idle"));
	m_pStateCom->Add_State(CFSM_Mon_Archer_Hit::Create(m_pDevice, m_pContext, Desc), TEXT("Monster_Archer_State_Hit"));
	m_pStateCom->Add_State(CFSM_Mon_Archer_Walk::Create(m_pDevice, m_pContext, Desc), TEXT("Monster_Archer_State_Walk"));
	m_pStateCom->Add_State(CFSM_Mon_Archer_Run::Create(m_pDevice, m_pContext, Desc), TEXT("Monster_Archer_State_Run"));
	m_pStateCom->Add_State(CFSM_Mon_Archer_Attack::Create(m_pDevice, m_pContext, Desc), TEXT("Monster_Archer_State_Attack"));
	
	return S_OK;
}

HRESULT CMon_Archer::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	/* Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxAnimMesh"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* Com_Model */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Monster_Archer"),
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	/* Com_State */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_State"),
		TEXT("Com_State"), reinterpret_cast<CComponent**>(&m_pStateCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CMon_Archer::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_vCamPosition", &m_pGameInstance->Get_CamPosition_Float4(), sizeof(_float4))))
		return E_FAIL;

	const LIGHT_DESC* pLightDesc = m_pGameInstance->Get_LightDesc(0);
	if (nullptr == pLightDesc)
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_vLightDir", &pLightDesc->vDirection, sizeof(_float4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_vLightDiffuse", &pLightDesc->vDiffuse, sizeof(_float4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_vLightAmbient", &pLightDesc->vDiffuse, sizeof(_float4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_vLightSpecular", &pLightDesc->vSpecular, sizeof(_float4))))
		return E_FAIL;

	return S_OK;
}

CMon_Archer* CMon_Archer::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CMon_Archer* pInstance = new CMon_Archer(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CMon_Archer"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CMon_Archer::Clone(void* pArg)
{
	CMon_Archer* pInstance = new CMon_Archer(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CMon_Archer"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMon_Archer::Free()
{
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pStateCom);
	Safe_Release(m_pModelCom);
	Safe_Release(m_pNavigationCom);

	for (size_t i = 0; i < COLL_END; i++)
		Safe_Release(m_pColliderCom[i]);

	__super::Free();
}
