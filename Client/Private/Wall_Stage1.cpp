#include "stdafx.h"

#include "Wall_Stage1.h"
#include "GameInstance.h"

#include "Monster.h"

CWall_Stage1::CWall_Stage1(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CElement { pDevice , pContext }
{
}

CWall_Stage1::CWall_Stage1(const CWall_Stage1& pPrototype)
	:CElement{ pPrototype }
{
}

HRESULT CWall_Stage1::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CWall_Stage1::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;
	
	Monster_Create();


	return S_OK;
}

void CWall_Stage1::Priority_Update(_float fTimeDelta)
{
}

void CWall_Stage1::Update(_float fTimeDelta)
{

}

void CWall_Stage1::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderObject(CRenderer::RG_NONBLEND, this);

}

HRESULT CWall_Stage1::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	_uint	iNumMeshes = m_pModelCom->Get_NumMeshes();

	for (size_t i = 0; i < iNumMeshes; i++)
	{
		if (FAILED(m_pModelCom->Bind_MaterialSRV(m_pShaderCom, aiTextureType_DIFFUSE, "g_DiffuseTexture", i)))
			return E_FAIL;

		if (FAILED(m_pModelCom->Bind_MaterialSRV(m_pShaderCom, aiTextureType_NORMALS, "g_NormalTexture", i)))
			return E_FAIL;

		m_pShaderCom->Begin(2);

		m_pModelCom->Render(i);
	}

	return S_OK;
}

HRESULT CWall_Stage1::Monster_Create()
{
	CMonster::MON_DESC Desc = {};
	Desc.iMaxHp = 10;
	Desc.iHp = 10;

	for (int i = 0; i < 3; ++i)
	{
		Desc.iNum = i;
		if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Mon_Bug"), TEXT("Layer_Monster_Stage1"),&Desc)))
			return E_FAIL;
	}

	Desc.iMaxHp = 400;
	Desc.iHp = 400;

	for (int i = 0; i < 2; ++i)
	{
		Desc.iNum = i;
		if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Mon_Archer"), TEXT("Layer_Monster_Stage1"), &Desc)))
			return E_FAIL;
	}

	Desc.iMaxHp = 400;
	Desc.iHp = 400;
	
	for (int i = 0; i < 1; ++i)
	{
		Desc.iNum = i;
		if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Mon_Knight"), TEXT("Layer_Monster_Stage1"), &Desc)))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CWall_Stage1::Ready_Components()
{
	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxMesh"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* For.Com_Model */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Wall_Stage1"),
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CWall_Stage1::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_pGameInstance->Get_Transform_Float4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;


	if (FAILED(m_pShaderCom->Bind_RawValue("g_vCamPosition", &m_pGameInstance->Get_CamPosition_Float4(), sizeof(_float4))))
		return E_FAIL;

	return S_OK;
}

CWall_Stage1* CWall_Stage1::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CWall_Stage1* pInstance = new CWall_Stage1(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CWall_State1"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CWall_Stage1::Clone(void* pArg)
{
	CWall_Stage1* pInstance = new CWall_Stage1(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CWall_State1"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CWall_Stage1::Free()
{
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModelCom);

	__super::Free();
}
