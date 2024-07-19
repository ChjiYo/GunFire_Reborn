#include "..\Public\Model.h"
#include "Mesh.h"
#include "Bone.h"
#include "Shader.h"
#include "Texture.h"
#include "Animation.h"
#include "Channel.h"

CModel::CModel(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CComponent{ pDevice, pContext }
{
}

CModel::CModel(const CModel& Prototype)
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

HRESULT CModel::Initialize_Prototype(enum TYPE eModelType, const _char* pModelFilePath, const _wstring& strBinFilePath, _fmatrix PreTransformMatrix)
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

	/* 바이너리화 */
	_ulong			dwByte = { 0 };

	HANDLE			hFile = CreateFile(strBinFilePath.c_str(), GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
	if (0 == hFile)
		return E_FAIL;

	if (FAILED(Write_Bones(dwByte, hFile)))
		return E_FAIL;

	if (FAILED(Write_Meshes(dwByte, hFile, PreTransformMatrix)))
		return E_FAIL;

	if (FAILED(Write_Materials(dwByte, hFile, pModelFilePath)))
		return E_FAIL;

	if (FAILED(Write_Animations(dwByte, hFile)))
		return E_FAIL;

	CloseHandle(hFile);

	return S_OK;
}

HRESULT CModel::Initialize(void* pArg)
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

HRESULT CModel::Bind_MaterialSRV(CShader* pShader, aiTextureType eType, const _char* pConstantName, _uint iMeshIndex)
{
	_uint		iMaterialIndex = m_Meshes[iMeshIndex]->Get_MaterialIndex();
	if (iMaterialIndex >= m_iNumMaterials)
		return E_FAIL;

	return m_Materials[iMaterialIndex].pMaterials[eType]->Bind_ShaderResource(pShader, pConstantName, 0);
}

HRESULT CModel::Bind_BoneMatrices(CShader* pShader, const _char* pConstantName, _uint iMeshIndex)
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
		CMesh* pMesh = CMesh::Create(m_pDevice, m_pContext, m_eModelType, m_pAIScene->mMeshes[i], m_Bones, PreTransformMatrix);
		if (nullptr == pMesh)
			return E_FAIL;

		m_Meshes.emplace_back(pMesh);
	}

	return S_OK;
}

HRESULT CModel::Ready_Materials(const _char* pModelFilePath)
{
	//갯수 저장
	m_iNumMaterials = m_pAIScene->mNumMaterials;

	//pModelFilePath 저장

	_char			szDrive[MAX_PATH] = {};
	_char			szDir[MAX_PATH] = {};

	_splitpath_s(pModelFilePath, szDrive, MAX_PATH, szDir, MAX_PATH, nullptr, 0, nullptr, 0);

	for (size_t i = 0; i < m_iNumMaterials; i++)
	{
		aiMaterial* pAIMaterial = m_pAIScene->mMaterials[i];
		MESHMATERIAL	MeshMaterial{};

		for (size_t j = 1; j < AI_TEXTURE_TYPE_MAX; j++)
		{
			aiString	strAIPath;

			if (FAILED(pAIMaterial->GetTexture(aiTextureType(j), 0, &strAIPath)))
				continue;

			m_vMatlIndex.push_back(j);

			_char			szFileName[MAX_PATH] = {};
			_char			szExt[MAX_PATH] = {};

			//char strAIPath.data 저장 
			//strcpy_s(AiPathData, strAIPath.data);
			
			m_arrAiPath[j] = new char[strlen(strAIPath.data) + 1];
			strcpy_s(m_arrAiPath[j], strlen(strAIPath.data) + 1, strAIPath.data);

			string Test = m_arrAiPath[j];
			vector<string> Src;
			
			
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

			++(MeshMaterial.iTextureCnt);
		}
		m_Materials.emplace_back(MeshMaterial);
	}
	return S_OK;
}

HRESULT CModel::Ready_Bones(const aiNode* pAINode, _int iParentIndex)
{
	CBone* pBone = CBone::Create(pAINode, iParentIndex);
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
		CAnimation* pAnimation = CAnimation::Create(m_pAIScene->mAnimations[i], m_Bones, m_KeyFrameIndices[i]);
		if (nullptr == pAnimation)
			return E_FAIL;

		m_Animations.emplace_back(pAnimation);
		int a = 10; 
	}

	return S_OK;
}

HRESULT CModel::Write_Bones(_ulong& dwByte, HANDLE& hWnd)
{
	_uint iNumBones = m_Bones.size();
	WriteFile(hWnd, &iNumBones, sizeof(_uint), &dwByte, nullptr);

	for (auto& iter : m_Bones)
	{
		_char* szName =  iter->Get_Desc().szName;
		_uint iNameLegnth = strlen(szName);

		WriteFile(hWnd, &iNameLegnth, sizeof(_uint), &dwByte, nullptr);
		WriteFile(hWnd, szName, sizeof(_char)* iNameLegnth, &dwByte, nullptr);

		_uint iParentIndex = iter->Get_Desc().iParentIndex;
		WriteFile(hWnd, &iParentIndex, sizeof(_uint) , &dwByte, nullptr);

		_float4x4 iTransformMatrix = iter->Get_Desc().TransformationMatrix;
		WriteFile(hWnd, &iTransformMatrix, sizeof(_float4x4), &dwByte, nullptr);
	}
	return S_OK;
}

HRESULT CModel::Write_Materials(_ulong& dwByte, HANDLE& hWnd, const _char* pModelFilePath)
{
	_uint iMatVecSize = (_uint)m_Materials.size();
	WriteFile(hWnd, &iMatVecSize, sizeof(_uint), &dwByte, nullptr);

	for(int i = 0 ; i < m_iNumMaterials ; ++i)
	{
		aiMaterial* pAIMaterial = m_pAIScene->mMaterials[i];

		MESHMATERIAL  MeshMaterial = {};
		
		_uint iiNumTexture = m_Materials.at(i).iTextureCnt;
		WriteFile(hWnd, &iiNumTexture, sizeof(_uint), &dwByte, nullptr);
		
		for (size_t k = 1; k < AI_TEXTURE_TYPE_MAX; ++k)
		{
			aiString   strTexturePath;

			if (FAILED(pAIMaterial->GetTexture(aiTextureType(k), 0, &strTexturePath)))
				continue;

			//_uint TexType = k;
			WriteFile(hWnd, &k, sizeof(_uint), &dwByte, nullptr);
			/* 얻어온 경로에서 신용할 수 있는 건 파일의 이름과 확장자 */

			// 내가 지정한 텍스처의 경로를 얻어온다.
			// 18개의 텍스처 타입 중에 없는 것이 있다면 continue로 다음 타입으로 넘어감

			_char   szFullPath[MAX_PATH] = {};

			/* 모델 파일 경로로부터 뜯어온 문자열 */
			_char   szDrive[MAX_PATH] = {};      // 드라이브 경로
			_char   szDir[MAX_PATH] = {};      // 디렉토리

			/* 모델 파일(aiScene)로부터 뜯어온 문자열 */
			_char   szFileName[MAX_PATH] = {};   // 파일 이름
			_char   szExt[MAX_PATH] = {};      // 확장자

			_splitpath_s(pModelFilePath, szDrive, MAX_PATH, szDir, MAX_PATH, nullptr, 0, nullptr, 0);
			_splitpath_s(strTexturePath.data, nullptr, 0, nullptr, 0, szFileName, MAX_PATH, szExt, MAX_PATH);

			strcpy_s(szFullPath, szDrive);      // szFullPath에 szDrive를 복사함
			strcat_s(szFullPath, szDir);      // szFullPath에 szDir을 붙임
			strcat_s(szFullPath, szFileName);   // szFullPath에 szFileName을 붙임
			strcat_s(szFullPath, szExt);      // szFullPath에 szExt를 붙임

			_tchar      szPerfectPath[MAX_PATH] = {};

			// _char형의 szFullPath를 _tchar형의 szPerfectPath로 치환함
			MultiByteToWideChar(CP_ACP, 0, szFullPath, strlen(szFullPath), szPerfectPath, MAX_PATH);

			_uint iPathLength = wcslen(szPerfectPath);
			WriteFile(hWnd, &iPathLength, sizeof(_uint), &dwByte, nullptr);
			WriteFile(hWnd, szPerfectPath, sizeof(_tchar) * iPathLength, &dwByte, nullptr);
			int a = 10;
		}
	}
	return S_OK;
}

HRESULT CModel::Write_Meshes(_ulong& dwByte, HANDLE& hWnd, _fmatrix PreTransformMatrix)
{
	_uint iNumMesh = m_iNumMeshes;
	WriteFile(hWnd, &iNumMesh, sizeof(iNumMesh), &dwByte, nullptr);

	for (auto iter : m_Meshes)
	{
		string strName = iter->Get_Desc().szName;
		_uint iMeshLength = strName.length();

		WriteFile(hWnd, &iMeshLength, sizeof(_uint), &dwByte, nullptr);
		WriteFile(hWnd, iter->Get_Desc().szName, iMeshLength, &dwByte, nullptr);

		_uint iMaterialIndex = iter->Get_Desc().iMaterialIndex;
		WriteFile(hWnd, &iMaterialIndex, sizeof(_uint), &dwByte, nullptr);

		_uint iNumVertices = iter->Get_Desc().iNumVertices;
		WriteFile(hWnd, &iNumVertices, sizeof(_uint), &dwByte, nullptr);

		_uint iNumIndices = iter->Get_Desc().iNumIndices;
		WriteFile(hWnd, &iNumIndices, sizeof(_uint), &dwByte, nullptr);

		_uint mNumFaces = iter->Get_Desc().mNumFaces;
		WriteFile(hWnd, &mNumFaces, sizeof(_uint), &dwByte, nullptr);

		vector<XMFLOAT3> vPosition = iter->Get_Desc().vPosition;
		_uint vPositionSize = vPosition.size();

		WriteFile(hWnd, &vPositionSize, sizeof(_uint), &dwByte, nullptr);
		WriteFile(hWnd, vPosition.data(), sizeof(XMFLOAT3) * vPosition.size(), &dwByte, nullptr);


		vector<XMFLOAT3> vNormal = iter->Get_Desc().vNormal;
		_uint vNormalSize = vNormal.size();

		WriteFile(hWnd, &vNormalSize, sizeof(_uint), &dwByte, nullptr);
		WriteFile(hWnd, vNormal.data(), sizeof(XMFLOAT3) * vNormal.size(), &dwByte, nullptr);

		vector<XMFLOAT3>vTexcoord = iter->Get_Desc().vTexcoord;
		_uint vTexcoordSize = vTexcoord.size();

		WriteFile(hWnd, &vTexcoordSize, sizeof(_uint), &dwByte, nullptr);
		WriteFile(hWnd, vTexcoord.data(), sizeof(XMFLOAT3) * vTexcoord.size(), &dwByte, nullptr);


		vector<XMFLOAT3> vTangent = iter->Get_Desc().vTangent;
		_uint vTangentSize = vTexcoord.size();

		WriteFile(hWnd, &vTangentSize, sizeof(_uint), &dwByte, nullptr);
		WriteFile(hWnd, vTangent.data(), sizeof(XMFLOAT3) * vTangent.size(), &dwByte, nullptr);


		vector<_uint> vIndices = iter->Get_Desc().vIndices;
		_uint vIndicesSize = vIndices.size();

		WriteFile(hWnd, &vIndicesSize, sizeof(_uint), &dwByte, nullptr);
		WriteFile(hWnd, vIndices.data(), sizeof(_uint) * vIndices.size(), &dwByte, nullptr);

		//Anim
		vector<XMUINT4> vBlendIndex = iter->Get_Desc().vBlendIndex;
		_uint vBlendIndexSize = vBlendIndex.size();

		WriteFile(hWnd, &vBlendIndexSize, sizeof(_uint), &dwByte, nullptr);
		WriteFile(hWnd, vBlendIndex.data(), sizeof(_uint) * vBlendIndex.size(), &dwByte, nullptr);


		vector<XMFLOAT4> vBlendWeight = iter->Get_Desc().vBlendWeight;
		_uint vBlendWeightSize = vBlendIndex.size(); 

		WriteFile(hWnd, &vBlendWeightSize, sizeof(_uint), &dwByte, nullptr);
		WriteFile(hWnd, vBlendWeight.data(), sizeof(_uint) * vBlendWeight.size(), &dwByte, nullptr);

		_uint  iNumBones = iter->Get_Desc().iNumBones;
		WriteFile(hWnd, &iNumBones, sizeof(_uint), &dwByte, nullptr);

		vector<_uint>	BoneIndices = iter->Get_Desc().BoneIndices;
		_uint			iBoneIndicesSize = BoneIndices.size();

		WriteFile(hWnd, &iBoneIndicesSize, sizeof(_uint), &dwByte, nullptr);
		WriteFile(hWnd, BoneIndices.data(), sizeof(_uint)* iBoneIndicesSize, &dwByte, nullptr);

		vector<_float4x4>	OffsetMatrices = iter->Get_Desc().OffsetMatrices;
		_uint			iOffsetMatricesSize = OffsetMatrices.size();

		WriteFile(hWnd, &iOffsetMatricesSize, sizeof(_uint), &dwByte, nullptr);
		WriteFile(hWnd, OffsetMatrices.data(), sizeof(_float4x4) * iOffsetMatricesSize, &dwByte, nullptr);

		vector<_uint>		mNumWeights = iter->Get_Desc().mNumWeights;
		_uint				mNumWeightsSize = mNumWeights.size();

		WriteFile(hWnd, &mNumWeightsSize, sizeof(_uint), &dwByte, nullptr);
		WriteFile(hWnd, mNumWeights.data(), sizeof(_uint) * mNumWeightsSize, &dwByte, nullptr);

		vector<_uint>	mVertexId = iter->Get_Desc().mVertexId;
		_uint			mVertexIdSize = mVertexId.size();
		
		WriteFile(hWnd, &mVertexIdSize, sizeof(_uint), &dwByte, nullptr);
		WriteFile(hWnd, mVertexId.data(), sizeof(_uint) * mVertexIdSize, &dwByte, nullptr);

		vector<size_t>	vecNumBones = iter->Get_Desc().vecNumBones;
		_uint			vecNumBonesSize = vecNumBones.size();

		WriteFile(hWnd, &vecNumBonesSize, sizeof(_uint), &dwByte, nullptr);
		WriteFile(hWnd, vecNumBones.data(), sizeof(size_t) * vecNumBonesSize, &dwByte, nullptr);

		vector<float>	mWeight = iter->Get_Desc().mWeight;
		_uint			mWeightSize = mWeight.size();

		WriteFile(hWnd, &mWeightSize, sizeof(_uint), &dwByte, nullptr);
		WriteFile(hWnd, mWeight.data(), sizeof(_uint)* mWeightSize, &dwByte, nullptr);

		//-----
		
		vector<VTXANIMMESH>	vAnimPosition = iter->Get_Desc().vecAnimMesh;
		_uint			vAnimPositionSize = vAnimPosition.size();

		WriteFile(hWnd, &vAnimPositionSize, sizeof(_uint), &dwByte, nullptr);
		WriteFile(hWnd, vAnimPosition.data(), sizeof(VTXANIMMESH)* vAnimPositionSize, &dwByte, nullptr);
		int a = 10;
	}

	return S_OK;
}

HRESULT CModel::Write_Animations(_ulong& dwByte, HANDLE& hWnd)
{
	WriteFile(hWnd, &m_iNumAnimations, sizeof(_uint), &dwByte, nullptr);

	//m_iNumChannels
	for(auto& iter :m_Animations)
	{
		vector<CChannel::CHANNEL_DESC>	Desc = iter->Get_ChDesc();
		_uint iDescSize = Desc.size();

		WriteFile(hWnd, &iDescSize, sizeof(_uint), &dwByte, nullptr);

		for (int i = 0; i < iDescSize; ++i)
		{
			_uint iChNameLength = strlen(Desc.at(i).szName);

			WriteFile(hWnd, &iChNameLength, sizeof(_uint), &dwByte, nullptr);
			WriteFile(hWnd, Desc.at(i).szName, sizeof(char) * iChNameLength, &dwByte, nullptr);

			vector<KEYFRAME> KeyFrames = Desc.at(i).KeyFrames;
			_uint KeyFramesSize = KeyFrames.size();

			WriteFile(hWnd, &KeyFramesSize, sizeof(_uint) , &dwByte, nullptr);
			WriteFile(hWnd, Desc.at(i).KeyFrames.data(), sizeof(KEYFRAME) * KeyFramesSize, &dwByte, nullptr);

			_uint iNumKeyFrames = Desc.at(i).iNumKeyFrames;
			WriteFile(hWnd, &iNumKeyFrames, sizeof(_uint), &dwByte, nullptr);

			_uint iNameLength = Desc.at(i).iNameLength;
			WriteFile(hWnd, &iNameLength, sizeof(_uint), &dwByte, nullptr);

			_uint iBoneIndex = Desc.at(i).iBoneIndex;
			WriteFile(hWnd, &iBoneIndex, sizeof(_uint), &dwByte, nullptr);
		}

		//WriteFile(hWnd, &iDescSize, sizeof(_uint), &dwByte, nullptr);
		//WriteFile(hWnd, Desc.data(), sizeof(CChannel::CHANNEL_DESC)* iDescSize, &dwByte, nullptr);
		/*WriteFile(hWnd, &iDescSize, sizeof(_uint), &dwByte, nullptr);
		WriteFile(hWnd, Desc.data(), sizeof(CChannel::CHANNEL_DESC) * iDescSize, &dwByte, nullptr);*/

		_float fDuration = iter->Get_Desc().fDuration;
		WriteFile(hWnd, &fDuration, sizeof(_float) , &dwByte, nullptr);

		_float fTickPerSecond = iter->Get_Desc().fTickPerSecond;
		WriteFile(hWnd, &fTickPerSecond, sizeof(_float), &dwByte, nullptr);

		_uint iNumChannels = iter->Get_Desc().iNumChannels;
		WriteFile(hWnd, &iNumChannels, sizeof(_uint), &dwByte, nullptr);

		char* szName = iter->Get_Desc().szName;
		_uint iNameLength = strlen(szName);

		WriteFile(hWnd, &iNameLength, sizeof(_uint), &dwByte, nullptr);
		WriteFile(hWnd, szName, sizeof(char) * iNameLength, &dwByte, nullptr);

	}

	return S_OK;
}

CModel* CModel::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, enum TYPE eModelType, const _char* pModelFilePath, const _wstring& strBinFilePath, _fmatrix PreTransformMatrix)
{
	CModel* pInstance = new CModel(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(eModelType, pModelFilePath, strBinFilePath, PreTransformMatrix)))
	{
		MSG_BOX(TEXT("Failed to Created : CModel"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CModel::Clone(void* pArg)
{
	CModel* pInstance = new CModel(*this);

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

	/*
	for (size_t j = 1; j < AI_TEXTURE_TYPE_MAX; j++)
{
    if (m_arrAiPath[j] != nullptr)
    {
        delete[] m_arrAiPath[j];
        m_arrAiPath[j] = nullptr;
    }
}

	*/

	m_Importer.FreeScene();
}

