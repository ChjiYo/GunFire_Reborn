#include "..\Public\BinModel.h"
#include "Mesh.h"
#include "Bone.h"
#include "Shader.h"
#include "Texture.h"
#include "Animation.h"

CBinModel::CBinModel(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CComponent{ pDevice, pContext }
{
}

CBinModel::CBinModel(const CBinModel& Prototype)
	: CComponent{ Prototype }
	, m_PreTransformMatrix{ Prototype.m_PreTransformMatrix }
	, m_iNumMeshes{ Prototype.m_iNumMeshes }
	, m_Meshes{ Prototype.m_Meshes }
	, m_iNumMaterials{ Prototype.m_iNumMaterials }
	, m_Materials{ Prototype.m_Materials }
	/*, m_Bones { Prototype.m_Bones }*/
	, m_iCurrentAnimationIndex{ Prototype.m_iCurrentAnimationIndex }
	, m_iNumAnimations{ Prototype.m_iNumAnimations }
	, m_Animations{ Prototype.m_Animations }
	, m_KeyFrameIndices{ Prototype.m_KeyFrameIndices }
	, m_eModelType{ Prototype.m_eModelType }

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

const _float4x4* CBinModel::Get_BoneMatrixPtr(const _char* pBoneName) const
{
	auto	iter = find_if(m_Bones.begin(), m_Bones.end(), [&](CBone* pBone) ->_bool
		{
			if (false == strcmp(pBoneName, pBone->Get_Name()))
				return true;

			return false;
		});

	return (*iter)->Get_CombinedTransformationMatrix();
}

HRESULT CBinModel::Initialize_Prototype(enum TYPE eModelType, const _wstring& pModelFilePath, _fmatrix PreTransformMatrix)
{
	/* aiProcess_PreTransformVertices : 각 메시가 배치되어야할 위치로 사전에 변환을 시켜 로드한다. */
	/* 피오나 : 칼, 피오나, 피오나배낭, 그림자. 메시들이 각각 배치되기위한 기초상태를 정의한 뼈라는 개념이 존재한다.  */
	_uint	iFlag = aiProcess_ConvertToLeftHanded | aiProcessPreset_TargetRealtime_Fast;

	if (TYPE_NONANIM == eModelType)
		iFlag |= aiProcess_PreTransformVertices;
	
	/* 지정해준 경로에 해당하는 파일의 정보를 읽어서 aiScene안에 저장해주는 역활. */
	/*m_pAIScene = m_Importer.ReadFile(pModelFilePath, iFlag);

	if (nullptr == m_pAIScene)
		return E_FAIL;*/

	_ulong			dwByte = { 0 };

	HANDLE			hFile = CreateFile(pModelFilePath.c_str(), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (0 == hFile)
		return E_FAIL;

	XMStoreFloat4x4(&m_PreTransformMatrix, PreTransformMatrix);

	m_eModelType = eModelType;

	if (FAILED(Ready_Bones(dwByte, hFile)))
		return E_FAIL;

	if (FAILED(Ready_Meshes(dwByte, hFile, PreTransformMatrix)))
		return E_FAIL;

	if (FAILED(Ready_Materials(dwByte, hFile)))
		return E_FAIL;

	if (FAILED(Ready_Animations(dwByte, hFile)))
		return E_FAIL;

	return S_OK;
}

HRESULT CBinModel::Initialize(void* pArg)
{


	return S_OK;
}

HRESULT CBinModel::Render(_uint iMeshIndex)
{
	m_Meshes[iMeshIndex]->Bind_Buffers();
	m_Meshes[iMeshIndex]->Render();

	return S_OK;
}

void CBinModel::Play_Animation(_float fTimeDelta)
{
	/* 내가 구동하고자했던 애니메이션(공격모션등등)이 사용하는 뼈(채널)들의 상태행렬을 보간하여 만들어주고 이 상태행려을 Cbone`s TransformationMatrix로 갱신해준다. */
	m_Animations[m_iCurrentAnimationIndex]->Update_TransformationMatrix(&m_fCurrentAnimPosition, fTimeDelta, m_Bones, m_isLoopAnim, m_KeyFrameIndices[m_iCurrentAnimationIndex]);

	/* 모든 뼈의 CombinedTransformationMatrix를 갱신한다. */
	for (auto& pBone : m_Bones)
	{
		pBone->Update_CombinedTransformationMatrix(m_Bones, XMLoadFloat4x4(&m_PreTransformMatrix));
		int a = 10;
	}
}

void CBinModel::SetUp_Animation(_uint iAnimationIndex, _bool isLoop)
{
	if (iAnimationIndex >= m_iNumAnimations)	
		return;
	
	for (int i = 0; i < m_KeyFrameIndices[iAnimationIndex].size(); ++i)
	{
		m_KeyFrameIndices[iAnimationIndex][i] = 0;
	}
	m_fCurrentAnimPosition = 0.f;
	m_isLoopAnim = isLoop;
	m_iCurrentAnimationIndex = iAnimationIndex; 
}

HRESULT CBinModel::Bind_MaterialSRV(CShader* pShader, aiTextureType eType, const _char* pConstantName, _uint iMeshIndex)
{
	_uint		iMaterialIndex = m_Meshes[iMeshIndex]->Get_MaterialIndex();
	if (iMaterialIndex >= m_iNumMaterials)
		return E_FAIL;

	return m_Materials[iMaterialIndex].pMaterials[eType]->Bind_ShaderResource(pShader, pConstantName, 0);
}

HRESULT CBinModel::Bind_BoneMatrices(CShader* pShader, const _char* pConstantName, _uint iMeshIndex)
{
	_float4x4		BoneMatrices[512] = {};

	m_Meshes[iMeshIndex]->SetUp_Matrices(BoneMatrices, m_Bones);

	pShader->Bind_Matrices(pConstantName, BoneMatrices, 512);

	return S_OK;
}

HRESULT CBinModel::Ready_Meshes(_ulong& dwByte, HANDLE& hFile,_fmatrix PreTransformMatrix)
{
	_uint NumMesh = 0;
	ReadFile(hFile, &NumMesh, sizeof(_uint), &dwByte, nullptr);
	m_iNumMeshes = NumMesh;
	for (size_t i = 0; i < NumMesh; i++)
	{
		CMesh::MESH_DESC Desc = {};

		Desc.szName;
		_uint iMeshLength = {};

		ReadFile(hFile, &iMeshLength, sizeof(_uint), &dwByte, nullptr);
		ReadFile(hFile, Desc.szName, iMeshLength, &dwByte, nullptr);

		Desc.iMaterialIndex;
		ReadFile(hFile, &Desc.iMaterialIndex, sizeof(_uint), &dwByte, nullptr);

		Desc.iNumVertices;
		ReadFile(hFile, &Desc.iNumVertices, sizeof(_uint), &dwByte, nullptr);

		Desc.iNumIndices;
		ReadFile(hFile, &Desc.iNumIndices, sizeof(_uint), &dwByte, nullptr);

		Desc.mNumFaces;
		ReadFile(hFile, &Desc.mNumFaces, sizeof(_uint), &dwByte, nullptr);

		Desc.vPosition = {};
		_uint vPositionSize = {};

		ReadFile(hFile, &vPositionSize, sizeof(_uint), &dwByte, nullptr);
		Desc.vPosition.resize(vPositionSize);
		ReadFile(hFile, Desc.vPosition.data(), sizeof(XMFLOAT3) * vPositionSize, &dwByte, nullptr);


		Desc.vNormal = {};
		_uint vNormalSize = {};

		ReadFile(hFile, &vNormalSize, sizeof(_uint), &dwByte, nullptr);
		Desc.vNormal.resize(vNormalSize);
		ReadFile(hFile, Desc.vNormal.data(), sizeof(XMFLOAT3) * vNormalSize, &dwByte, nullptr);

		Desc.vTexcoord = {};
		_uint vTexcoordSize = {};

		ReadFile(hFile, &vTexcoordSize, sizeof(_uint), &dwByte, nullptr);
		Desc.vTexcoord.resize(vTexcoordSize);
		ReadFile(hFile, Desc.vTexcoord.data(), sizeof(XMFLOAT3) * vTexcoordSize, &dwByte, nullptr);


		Desc.vTangent = {};
		_uint vTangentSize = {};

		ReadFile(hFile, &vTangentSize, sizeof(_uint), &dwByte, nullptr);
		Desc.vTangent.resize(vTangentSize);
		ReadFile(hFile, Desc.vTangent.data(), sizeof(XMFLOAT3) * vTangentSize, &dwByte, nullptr);


		Desc.vIndices = {};
		_uint vIndicesSize = {};

		ReadFile(hFile, &vIndicesSize, sizeof(_uint), &dwByte, nullptr);
		Desc.vIndices.resize(vIndicesSize);
		ReadFile(hFile, Desc.vIndices.data(), sizeof(_uint) * vIndicesSize, &dwByte, nullptr);

		//Anim
		Desc.vBlendIndex = {};
		_uint vBlendIndexSize = {};

		ReadFile(hFile, &vBlendIndexSize, sizeof(_uint), &dwByte, nullptr);
		Desc.vBlendIndex.resize(vBlendIndexSize);
		ReadFile(hFile, Desc.vBlendIndex.data(), sizeof(_uint) * vBlendIndexSize, &dwByte, nullptr);

		Desc.vBlendWeight;
		_uint vBlendWeightSize;

		ReadFile(hFile, &vBlendWeightSize, sizeof(_uint), &dwByte, nullptr);
		Desc.vBlendWeight.resize(vBlendWeightSize);
		ReadFile(hFile, Desc.vBlendWeight.data(), sizeof(_uint) * vBlendWeightSize, &dwByte, nullptr);

		Desc.iNumBones;
		ReadFile(hFile, &Desc.iNumBones, sizeof(_uint), &dwByte, nullptr);

		Desc.BoneIndices;
		_uint			iBoneIndicesSize;

		ReadFile(hFile, &iBoneIndicesSize, sizeof(_uint), &dwByte, nullptr);
		Desc.BoneIndices.resize(iBoneIndicesSize);
		ReadFile(hFile, Desc.BoneIndices.data(), sizeof(_uint) * iBoneIndicesSize, &dwByte, nullptr);

		Desc.OffsetMatrices;
		_uint			iOffsetMatricesSize;

		ReadFile(hFile, &iOffsetMatricesSize, sizeof(_uint), &dwByte, nullptr);
		Desc.OffsetMatrices.resize(iOffsetMatricesSize);
		ReadFile(hFile, Desc.OffsetMatrices.data(), sizeof(_float4x4) * iOffsetMatricesSize, &dwByte, nullptr);

		Desc.mNumWeights;
		_uint				mNumWeightsSize;

		ReadFile(hFile, &mNumWeightsSize, sizeof(_uint), &dwByte, nullptr);
		Desc.mNumWeights.resize(mNumWeightsSize);
		ReadFile(hFile, Desc.mNumWeights.data(), sizeof(_uint) * mNumWeightsSize, &dwByte, nullptr);

		Desc.mVertexId = {};
		_uint			mVertexIdSize = {};

		ReadFile(hFile, &mVertexIdSize, sizeof(_uint), &dwByte, nullptr);
		Desc.mVertexId.resize(mVertexIdSize);
		int a = 10;
		ReadFile(hFile, Desc.mVertexId.data(), sizeof(_uint) * mVertexIdSize, &dwByte, nullptr);

		Desc.vecNumBones;
		_uint			vecNumBonesSize;

		ReadFile(hFile, &vecNumBonesSize, sizeof(_uint), &dwByte, nullptr);
		Desc.vecNumBones.resize(vecNumBonesSize);
		ReadFile(hFile, Desc.vecNumBones.data(), sizeof(size_t) * vecNumBonesSize, &dwByte, nullptr);

		Desc.mWeight;
		_uint			mWeightSize;

		ReadFile(hFile, &mWeightSize, sizeof(_uint), &dwByte, nullptr);
		Desc.mWeight.resize(mWeightSize);
		ReadFile(hFile, Desc.mWeight.data(), sizeof(_uint) * mWeightSize, &dwByte, nullptr);

		//------

		Desc.vecAnimMesh = {};
		_uint			vAnimPositionSize = {};

		ReadFile(hFile, &vAnimPositionSize, sizeof(_uint), &dwByte, nullptr);
		Desc.vecAnimMesh.resize(vAnimPositionSize);
		ReadFile(hFile, Desc.vecAnimMesh.data(), sizeof(VTXANIMMESH)* vAnimPositionSize, &dwByte, nullptr);
		
		int sda = 10;


		CModel::TYPE eType = static_cast<CModel::TYPE>(m_eModelType);
		
		/* 메시를 하나씩 만든다. == VertexBuffer ,IndexBuffer를 만드낟. */
		CMesh* pMesh = CMesh::Create(m_pDevice, m_pContext, eType, Desc, m_Bones, PreTransformMatrix);
		if (nullptr == pMesh)
			return E_FAIL;

		m_Meshes.emplace_back(pMesh);
	}

	return S_OK;
}

HRESULT CBinModel::Ready_Materials(_ulong& dwByte, HANDLE& hFile)
{
	_uint iMatVecSize = 0;
	ReadFile(hFile, &iMatVecSize, sizeof(_uint), &dwByte, nullptr);
	m_iNumMaterials = iMatVecSize;
	//for(int i = 0 ;  i)
	//iiNumTexture
	for (size_t i = 0; i < iMatVecSize; ++i)
	{
		_uint iTextureCnt = 0;
		ReadFile(hFile, &iTextureCnt, sizeof(_uint), &dwByte, nullptr);

		MESHMATERIAL			MeshMaterial = {};

		for (int k = 0; k < iTextureCnt; ++k)
		{
			_uint iPathLength = 0;
			_tchar szPerfectPath[MAX_PATH] = {};

			_uint iTexType = {};
			ReadFile(hFile, &iTexType, sizeof(_uint), &dwByte, nullptr);



			ReadFile(hFile, &iPathLength, sizeof(_uint), &dwByte, nullptr);
			ReadFile(hFile, szPerfectPath, sizeof(_tchar) * iPathLength, &dwByte, nullptr);
			szPerfectPath[iPathLength] = '\0';

			MeshMaterial.pMaterials[iTexType] = CTexture::Create(m_pDevice, m_pContext, szPerfectPath, 1);
			if (nullptr == MeshMaterial.pMaterials[iTexType])
				return E_FAIL;
		}
		m_Materials.emplace_back(MeshMaterial);
		
	}
}

HRESULT CBinModel::Ready_Bones(_ulong& dwByte, HANDLE& hFile)
{
	_uint iNumBones = {};
	ReadFile(hFile, &iNumBones, sizeof(_uint), &dwByte, nullptr);

	for (int i = 0; i < iNumBones; ++i)
	{
		_char szName[MAX_PATH] = {};
		_uint iNameLegnth;

		ReadFile(hFile, &iNameLegnth, sizeof(_uint), &dwByte, nullptr);
		ReadFile(hFile, szName, sizeof(_char) * iNameLegnth, &dwByte, nullptr);
		szName[iNameLegnth] = '\0';

		_uint iParentIndex = {};
		ReadFile(hFile, &iParentIndex, sizeof(_uint), &dwByte, nullptr);

		_float4x4 iTransformMatrix = {};
		ReadFile(hFile, &iTransformMatrix, sizeof(_float4x4), &dwByte, nullptr);

		CBone::BONE_DESC	Desc = {};
		Desc.iParentIndex = iParentIndex;
		Desc.szName = szName;
		Desc.TransformationMatrix = iTransformMatrix;

		CBone* pBone = CBone::Create(Desc, iParentIndex);

		if (nullptr == pBone)
			return E_FAIL;

		m_Bones.emplace_back(pBone);
	}

	return S_OK;
}

HRESULT CBinModel::Ready_Animations(_ulong& dwByte, HANDLE& hFile)
{
	_uint iNumAnimations = {};
	ReadFile(hFile, &iNumAnimations, sizeof(_uint), &dwByte, nullptr);
	m_iNumAnimations = iNumAnimations;

	/* vector<_uint> 애니메이션 갯수만큼 할당한다. */
	m_KeyFrameIndices.resize(m_iNumAnimations);

	for (size_t i = 0; i < m_iNumAnimations; i++)
	{
		vector<CChannel::CHANNEL_DESC>	Desc;
		_uint iDescSize = {};
		ReadFile(hFile, &iDescSize, sizeof(_uint), &dwByte, nullptr);
		Desc.resize(iDescSize);
		for (int k = 0; k < iDescSize; ++k)
		{
			//m_arrAiPath[j] = new char[strlen(strAIPath.data) + 1];
			//strcpy_s(m_arrAiPath[j], strlen(strAIPath.data) + 1, strAIPath.data);

			_uint iChNameLength = {};

			ReadFile(hFile, &iChNameLength, sizeof(_uint), &dwByte, nullptr);
			char* pTest = new char[iChNameLength + 1];
			
			ReadFile(hFile, pTest, sizeof(_char) * iChNameLength, &dwByte, nullptr);
			pTest[iChNameLength] = '\0';

			Desc.at(k).szName = pTest;

			Desc.at(k).KeyFrames = {};
			_uint KeyFramesSize = {};

			ReadFile(hFile, &KeyFramesSize, sizeof(_uint), &dwByte, nullptr);
			Desc.at(k).KeyFrames.resize(KeyFramesSize);
			ReadFile(hFile, Desc.at(k).KeyFrames.data(), sizeof(KEYFRAME) * KeyFramesSize, &dwByte, nullptr);

			Desc.at(k).iNumKeyFrames = {};
			ReadFile(hFile, &(Desc.at(k).iNumKeyFrames), sizeof(_uint), &dwByte, nullptr);

			Desc.at(k).iNameLength = {};
			ReadFile(hFile, &(Desc.at(k).iNameLength), sizeof(_uint), &dwByte, nullptr);

			Desc.at(k).iBoneIndex = {};
			ReadFile(hFile, &(Desc.at(k).iBoneIndex), sizeof(_uint), &dwByte, nullptr);
		}

	
		CAnimation::ANIM_DESC Anim_Desc = {};

		Anim_Desc.fDuration;
		ReadFile(hFile, &Anim_Desc.fDuration, sizeof(_float), &dwByte, nullptr);

		Anim_Desc.fTickPerSecond;
		ReadFile(hFile, &Anim_Desc.fTickPerSecond, sizeof(_float), &dwByte, nullptr);

		Anim_Desc.iNumChannels ;
		ReadFile(hFile, &Anim_Desc.iNumChannels, sizeof(_uint), &dwByte, nullptr);

		char szName[MAX_PATH] = {};
		_uint iNameLength;

		ReadFile(hFile, &iNameLength, sizeof(_uint), &dwByte, nullptr);
		ReadFile(hFile, szName, sizeof(char) * iNameLength, &dwByte, nullptr);
		szName[iNameLength] = '\0';

		Anim_Desc.szName = szName;

		CAnimation* pAnimation = CAnimation::Create(Desc, Anim_Desc, m_Bones, m_KeyFrameIndices[i]);
		if (nullptr == pAnimation)
			return E_FAIL;

		m_Animations.emplace_back(pAnimation);
	}

	return S_OK;
}

CBinModel* CBinModel::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, enum TYPE eModelType, const _wstring& pModelFilePath, _fmatrix PreTransformMatrix)
{
	CBinModel* pInstance = new CBinModel(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(eModelType, pModelFilePath, PreTransformMatrix)))
	{
		MSG_BOX(TEXT("Failed to Created : CBinModel"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CBinModel::Clone(void* pArg)
{
	CBinModel* pInstance = new CBinModel(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CBinModel"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBinModel::Free()
{

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
	__super::Free();
}

