#include "..\Public\Model.h"
#include "Mesh.h"
#include "Bone.h"
#include "Shader.h"
#include "Texture.h"
#include "Animation.h"

CModel::CModel(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CComponent { pDevice, pContext }
{
}

CModel::CModel(const CModel & Prototype)
	: CComponent { Prototype }
	, m_PreTransformMatrix { Prototype.m_PreTransformMatrix }
	, m_iNumMeshes { Prototype.m_iNumMeshes }
	, m_Meshes { Prototype.m_Meshes }
	, m_iNumMaterials { Prototype.m_iNumMaterials}
	, m_Materials { Prototype.m_Materials }
	/*, m_Bones { Prototype.m_Bones }*/
	, m_iCurrentAnimationIndex{ Prototype.m_iCurrentAnimationIndex }
	, m_iNumAnimations {Prototype.m_iNumAnimations }	
	, m_Animations{ Prototype.m_Animations }
	, m_KeyFrameIndices {Prototype.m_KeyFrameIndices }
{
	

	for (auto& pPrototypeBone : Prototype.m_Bones)
		m_Bones.emplace_back(pPrototypeBone->Clone());

	for (auto& pAnimation : m_Animations)
		Safe_AddRef(pAnimation);

	for (auto& Material : m_Materials)
	{
		for (size_t i = 0; i < AI_TEXTURE_TYPE_MAX; i++)
		{
			Safe_AddRef(Material.pMaterials[i]);
		}
	}

	for (auto& pMesh : m_Meshes)
		Safe_AddRef(pMesh);
}

const _float4x4* CModel::Get_BoneMatrixPtr(const _char* pBoneName) const
{
	auto	iter = find_if(m_Bones.begin(), m_Bones.end(), [&](CBone* pBone) ->_bool
		{
			if (false == strcmp(pBoneName, pBone->Get_Name()))
				return true;

			return false;
		});

	return (*iter)->Get_CombinedTransformationMatrix();
}

HRESULT CModel::Initialize_Prototype(enum TYPE eModelType, const _char* pModelFilePath, _fmatrix PreTransformMatrix)
{
	/* aiProcess_PreTransformVertices : 각 메시가 배치되어야할 위치로 사전에 변환을 시켜 로드한다. */
	/* 피오나 : 칼, 피오나, 피오나배낭, 그림자. 메시들이 각각 배치되기위한 기초상태를 정의한 뼈라는 개념이 존재한다.  */
	_uint	iFlag = aiProcess_ConvertToLeftHanded | aiProcessPreset_TargetRealtime_Fast;

	if (TYPE_NONANIM == eModelType)	
		iFlag |= aiProcess_PreTransformVertices;	

	/* 지정해준 경로에 해당하는 파일의 정보를 읽어서 aiScene안에 저장해주는 역활. */
	m_pAIScene = m_Importer.ReadFile(pModelFilePath, iFlag);

	if (nullptr == m_pAIScene)
		return E_FAIL;	

	XMStoreFloat4x4(&m_PreTransformMatrix, PreTransformMatrix);

	m_eModelType = eModelType;

	if (FAILED(Ready_Bones(m_pAIScene->mRootNode)))
		return E_FAIL;

	if (FAILED(Ready_Meshes(PreTransformMatrix)))
		return E_FAIL;

	if (FAILED(Ready_Materials(pModelFilePath)))
		return E_FAIL;

	if (FAILED(Ready_Animations()))
		return E_FAIL;

	return S_OK;
}

HRESULT CModel::Initialize(void * pArg)
{


	return S_OK;
}

HRESULT CModel::Render(_uint iMeshIndex)
{
	m_Meshes[iMeshIndex]->Bind_Buffers();
	m_Meshes[iMeshIndex]->Render();

	return S_OK;
}

void CModel::Play_Animation(_float fTimeDelta)
{
	/* 내가 구동하고자했던 애니메이션(공격모션등등)이 사용하는 뼈(채널)들의 상태행렬을 보간하여 만들어주고 이 상태행려을 Cbone`s TransformationMatrix로 갱신해준다. */
	m_Animations[m_iCurrentAnimationIndex]->Update_TransformationMatrix(&m_fCurrentAnimPosition, fTimeDelta, m_Bones, m_isLoopAnim, m_KeyFrameIndices[m_iCurrentAnimationIndex]);

	/* 모든 뼈의 CombinedTransformationMatrix를 갱신한다. */
	for (auto& pBone : m_Bones)	
		pBone->Update_CombinedTransformationMatrix(m_Bones, XMLoadFloat4x4(&m_PreTransformMatrix));

}

void CModel::SetUp_Animation(_uint iAnimationIndex, _bool isLoop)
{
	if (iAnimationIndex >= m_iNumAnimations)
		return;

	for (int i = 0; i < m_iNumAnimations; ++i)
		m_KeyFrameIndices[iAnimationIndex][i] = 0;

	m_fCurrentAnimPosition = 0.f;
	m_isLoopAnim = isLoop;
	m_iCurrentAnimationIndex = iAnimationIndex;
}

HRESULT CModel::Bind_MaterialSRV(CShader * pShader, aiTextureType eType, const _char * pConstantName, _uint iMeshIndex)
{
	_uint		iMaterialIndex = m_Meshes[iMeshIndex]->Get_MaterialIndex();
	if (iMaterialIndex >= m_iNumMaterials)
		return E_FAIL;

	return m_Materials[iMaterialIndex].pMaterials[eType]->Bind_ShaderResource(pShader, pConstantName, 0);	
}

HRESULT CModel::Bind_BoneMatrices(CShader * pShader, const _char * pConstantName, _uint iMeshIndex)
{
	_float4x4		BoneMatrices[512] = {};

	m_Meshes[iMeshIndex]->SetUp_Matrices(BoneMatrices, m_Bones);

	pShader->Bind_Matrices(pConstantName, BoneMatrices, 512);

	return S_OK;
}

HRESULT CModel::Ready_Meshes(_fmatrix PreTransformMatrix)
{
	m_iNumMeshes = m_pAIScene->mNumMeshes;

	for (size_t i = 0; i < m_iNumMeshes; i++)
	{
		/* 메시를 하나씩 만든다. == VertexBuffer ,IndexBuffer를 만드낟. */
		CMesh*		pMesh = CMesh::Create(m_pDevice, m_pContext, m_eModelType, m_pAIScene->mMeshes[i], m_Bones, PreTransformMatrix);
		if (nullptr == pMesh)
			return E_FAIL;

		m_Meshes.emplace_back(pMesh);
	}
	
	return S_OK;
}

HRESULT CModel::Ready_Materials(const _char* pModelFilePath)
{
	m_iNumMaterials = m_pAIScene->mNumMaterials;

	_char			szDrive[MAX_PATH] = {};
	_char			szDir[MAX_PATH] = {};

	_splitpath_s(pModelFilePath, szDrive, MAX_PATH, szDir, MAX_PATH, nullptr, 0, nullptr, 0);

	for (size_t i = 0; i < m_iNumMaterials; i++)
	{
		aiMaterial*		pAIMaterial = m_pAIScene->mMaterials[i];
		MESHMATERIAL	MeshMaterial{};

		for (size_t j = 1; j < AI_TEXTURE_TYPE_MAX; j++)
		{			
			aiString	strAIPath;			

			if (FAILED(pAIMaterial->GetTexture(aiTextureType(j), 0, &strAIPath)))
				continue;	

			_char			szFileName[MAX_PATH] = {};
			_char			szExt[MAX_PATH] = {};

			_splitpath_s(strAIPath.data, nullptr, 0, nullptr, 0, szFileName, MAX_PATH, szExt, MAX_PATH);

			_char			szFullPath[MAX_PATH] = {};

			strcpy_s(szFullPath, szDrive);
			strcat_s(szFullPath, szDir);
			strcat_s(szFullPath, szFileName);
			strcat_s(szFullPath, szExt);

			_tchar			szPerfectFullPath[MAX_PATH] = {};

			MultiByteToWideChar(CP_ACP, 0, szFullPath, strlen(szFullPath), szPerfectFullPath, MAX_PATH);

			MeshMaterial.pMaterials[j] = CTexture::Create(m_pDevice, m_pContext, szPerfectFullPath, 1);
			if (nullptr == MeshMaterial.pMaterials[j])
				return E_FAIL;		
		}
		m_Materials.emplace_back(MeshMaterial);
	}
	return S_OK;
}

HRESULT CModel::Ready_Bones(const aiNode* pAINode, _int iParentIndex)
{
	CBone*		pBone = CBone::Create(pAINode, iParentIndex);
	if (nullptr == pBone)
		return E_FAIL;

	m_Bones.emplace_back(pBone);

	_uint		iNumChildren = pAINode->mNumChildren;

	_int		iParentBoneIndex = m_Bones.size() - 1;

	for (size_t i = 0; i < iNumChildren; i++)	
		Ready_Bones(pAINode->mChildren[i], iParentBoneIndex);

	return S_OK;
}

HRESULT CModel::Ready_Animations()
{
	m_iNumAnimations = m_pAIScene->mNumAnimations;

	/* vector<_uint> 애니메이션 갯수만큼 할당한다. */
	m_KeyFrameIndices.resize(m_iNumAnimations);

	for (size_t i = 0; i < m_iNumAnimations; i++)
	{
		/* _uint : 키프레임인덱스를 애니메이션 사용하는 뼈 갯수만큼 확보해둔다. */
		CAnimation*		pAnimation = CAnimation::Create(m_pAIScene->mAnimations[i], m_Bones, m_KeyFrameIndices[i]);
		if (nullptr == pAnimation)
			return E_FAIL;

		
		

		m_Animations.emplace_back(pAnimation);
	}



	return S_OK;
}

CModel * CModel::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, enum TYPE eModelType, const _char* pModelFilePath, _fmatrix PreTransformMatrix)
{
	CModel*		pInstance = new CModel(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(eModelType, pModelFilePath, PreTransformMatrix)))
	{
		MSG_BOX(TEXT("Failed to Created : CModel"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CModel::Clone(void * pArg)
{
	CModel*		pInstance = new CModel(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CModel"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CModel::Free()
{
	__super::Free();

	for (auto& pAnimation : m_Animations)
		Safe_Release(pAnimation);

	for (auto& pBone : m_Bones)
		Safe_Release(pBone);

	m_Bones.clear();

	for (auto& Material : m_Materials)
	{
		for (size_t i = 0; i < AI_TEXTURE_TYPE_MAX; i++)		
			Safe_Release(Material.pMaterials[i]);
	}

	m_Materials.clear();

	for (auto& pMesh : m_Meshes)
		Safe_Release(pMesh);

	m_Importer.FreeScene();
}

