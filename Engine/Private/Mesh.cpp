#include "..\Public\Mesh.h"
#include "Bone.h"

CMesh::CMesh(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CVIBuffer { pDevice, pContext }
{

}

HRESULT CMesh::Initialize_Prototype(CModel::TYPE eModelType, const aiMesh * pAIMesh, const vector<CBone*>& Bones, _fmatrix PreTransformMatrix)
{
	strcpy_s(m_szName, pAIMesh->mName.data);

	m_iMaterialIndex = pAIMesh->mMaterialIndex;
	m_iNumVertexBuffers = 1;
	m_iNumVertices = pAIMesh->mNumVertices;
	m_iVertexStride = eModelType == CModel::TYPE_NONANIM ? sizeof(VTXMESH) : sizeof(VTXANIMMESH);

	m_iNumIndices = pAIMesh->mNumFaces * 3;
	m_iIndexStride = 4;
	m_eIndexFormat = DXGI_FORMAT_R32_UINT;
	m_ePrimitive_Topology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

#pragma region VERTEX_BUFFER
	m_BufferDesc.ByteWidth = m_iNumVertices * m_iVertexStride;
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT; /* 정적할당한다. 추후 우리가 직접 정점버퍼의 공간을 변경할 일이 없다. */
	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;
	m_BufferDesc.StructureByteStride = m_iVertexStride;

	/* 장치가 할당해주는 공간에 채워넣어야할 값들을 내가 미리 준비한다. */

	void*			pVertices = 
		eModelType == CModel::TYPE_NONANIM ? Create_NonAnimVertices(pAIMesh, PreTransformMatrix) : Create_AnimVertices(pAIMesh, Bones);
	if (nullptr == pVertices)
		return E_FAIL;

	ZeroMemory(&m_InitialData, sizeof m_InitialData);
	m_InitialData.pSysMem = pVertices;

	if (FAILED(__super::Create_Buffer(&m_pVB)))
		return E_FAIL;

	Safe_Delete_Array(pVertices);

#pragma endregion

#pragma region INDEX_BUFFER
	m_BufferDesc.ByteWidth = m_iNumIndices * m_iIndexStride;
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT; /* 정적할당한다. 추후 우리가 직접 정점버퍼의 공간을 변경할 일이 없다. */
	m_BufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;
	m_BufferDesc.StructureByteStride = /*m_iIndexStride*/0;

	_uint*	pIndices = new _uint[m_iNumIndices];
	ZeroMemory(pIndices, sizeof(_uint) * m_iNumIndices);

	_uint	iNumIndices = { 0 };

	/* 삼각형 갯수만큼 루프돈다. */
	for (size_t i = 0; i < pAIMesh->mNumFaces; i++)
	{
		aiFace		AIFace = pAIMesh->mFaces[i];

		pIndices[iNumIndices++] = AIFace.mIndices[0];
		pIndices[iNumIndices++] = AIFace.mIndices[1];
		pIndices[iNumIndices++] = AIFace.mIndices[2];
	}

	ZeroMemory(&m_InitialData, sizeof m_InitialData);
	m_InitialData.pSysMem = pIndices;

	if (FAILED(__super::Create_Buffer(&m_pIB)))
		return E_FAIL;

	Safe_Delete_Array(pIndices);

#pragma endregion


	return S_OK;
}

void CMesh::SetUp_Matrices(_float4x4 * pMatrices, const vector<class CBone*>& Bones)
{
	for (size_t i = 0; i < m_iNumBones; i++)
	{
		XMStoreFloat4x4(&pMatrices[i], XMLoadFloat4x4(&m_OffsetMatrices[i]) * XMLoadFloat4x4(Bones[m_BoneIndices[i]]->Get_CombinedTransformationMatrix()));
	}
}

void * CMesh::Create_NonAnimVertices(const aiMesh* pAIMesh, _fmatrix PreTransformMatrix)
{
	VTXMESH*			pVertices = new VTXMESH[m_iNumVertices];
	ZeroMemory(pVertices, sizeof(VTXMESH) * m_iNumVertices);

	for (size_t i = 0; i < m_iNumVertices; i++)
	{
		memcpy(&pVertices[i].vPosition, &pAIMesh->mVertices[i], sizeof(_float3));
		XMStoreFloat3(&pVertices[i].vPosition, XMVector3TransformCoord(XMLoadFloat3(&pVertices[i].vPosition), PreTransformMatrix));

		memcpy(&pVertices[i].vNormal, &pAIMesh->mNormals[i], sizeof(_float3));
		XMStoreFloat3(&pVertices[i].vNormal, XMVector3TransformNormal(XMLoadFloat3(&pVertices[i].vNormal), PreTransformMatrix));

		/* i번째 정점의 0번째 Texcoord를 가져오겠다. why? 정점은 ㅔㅌㄱ스쿠드를 여덟개 가질 수 있으니까. */
		memcpy(&pVertices[i].vTexcoord, &pAIMesh->mTextureCoords[0][i], sizeof(_float2));
		memcpy(&pVertices[i].vTangent, &pAIMesh->mTangents[i], sizeof(_float3));
	}
	return pVertices;
}

void * CMesh::Create_AnimVertices(const aiMesh* pAIMesh, const vector<CBone*>& Bones)
{
	VTXANIMMESH*			pVertices = new VTXANIMMESH[m_iNumVertices];
	ZeroMemory(pVertices, sizeof(VTXANIMMESH) * m_iNumVertices);

	for (size_t i = 0; i < m_iNumVertices; i++)
	{
		memcpy(&pVertices[i].vPosition, &pAIMesh->mVertices[i], sizeof(_float3));
		memcpy(&pVertices[i].vNormal, &pAIMesh->mNormals[i], sizeof(_float3));				
		memcpy(&pVertices[i].vTexcoord, &pAIMesh->mTextureCoords[0][i], sizeof(_float2));
		memcpy(&pVertices[i].vTangent, &pAIMesh->mTangents[i], sizeof(_float3));		
	}

	/* 이 메시의 정점들에게 영향을 주는 뼈의 갯수. */
	m_iNumBones = pAIMesh->mNumBones;

	for (size_t i = 0; i < m_iNumBones; i++)
	{
		aiBone*		pAIBone = pAIMesh->mBones[i];

		_uint		iBoneIndex = {};

		auto	iter = find_if(Bones.begin(), Bones.end(), [&](CBone* pBone) {
			if (false == strcmp(pAIBone->mName.data, pBone->Get_Name()))			
				return true;		
			++iBoneIndex;
			return false;
		});

		m_BoneIndices.emplace_back(iBoneIndex);

		_float4x4		OffsetMatrix;
		memcpy(&OffsetMatrix, &pAIBone->mOffsetMatrix, sizeof(_float4x4));

		XMStoreFloat4x4(&OffsetMatrix, XMMatrixTranspose(XMLoadFloat4x4(&OffsetMatrix)));
		
		m_OffsetMatrices.emplace_back(OffsetMatrix);		

		for (size_t j = 0; j < pAIBone->mNumWeights; j++)
		{
			aiVertexWeight	AIWeight = pAIBone->mWeights[j];

			/* i번째 뼈가 영향을 주는 정점의 갯수는 pAIBone->mNumWeights(j-> 0, 1 ~~~) */
			/* i번째 뼈가 영향을 주는 정점들 중, j번째 정점의 인덱스 AIWeight.mVertexId */

			if(0.f == pVertices[AIWeight.mVertexId].vBlendWeight.x)
			{
				pVertices[AIWeight.mVertexId].vBlendIndex.x = i;
				pVertices[AIWeight.mVertexId].vBlendWeight.x = AIWeight.mWeight;
			}

			else if(0.f == pVertices[AIWeight.mVertexId].vBlendWeight.y)
			{
				pVertices[AIWeight.mVertexId].vBlendIndex.y = i;
				pVertices[AIWeight.mVertexId].vBlendWeight.y = AIWeight.mWeight;
			}

			else if (0.f == pVertices[AIWeight.mVertexId].vBlendWeight.z)
			{
				pVertices[AIWeight.mVertexId].vBlendIndex.z = i;
				pVertices[AIWeight.mVertexId].vBlendWeight.z = AIWeight.mWeight;
			}
			
			else
			{
				pVertices[AIWeight.mVertexId].vBlendIndex.w = i;
				pVertices[AIWeight.mVertexId].vBlendWeight.w = AIWeight.mWeight;
			}			
		}
	}

	if (0 == m_iNumBones)
	{
		/* 이 메시가 제대로된 위치에 배치되서 그려질 수 있도록. */
		/* 이 메시의 위치등의 상태를 표현하고 있ㄴ느 뼈를 찾을거야. */
		/* 메시와 이름이같은 뼈를 찾아주면 된다,. */
		m_iNumBones = 1;

		_uint		iBoneIndex = {};

		auto	iter = find_if(Bones.begin(), Bones.end(), [&](CBone* pBone) {
			if (false == strcmp(m_szName, pBone->Get_Name()))
				return true;
			++iBoneIndex;
			return false;
		});

		m_BoneIndices.emplace_back(iBoneIndex);

		_float4x4		OffsetMatrix{};

		XMStoreFloat4x4(&OffsetMatrix, XMMatrixIdentity());

		m_OffsetMatrices.emplace_back(OffsetMatrix);

	}

	return pVertices;	
}

CMesh * CMesh::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, CModel::TYPE eModelType, const aiMesh * pAIMesh, const vector<class CBone*>& Bones, _fmatrix PreTransformMatrix)
{
	CMesh*		pInstance = new CMesh(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(eModelType, pAIMesh, Bones, PreTransformMatrix)))
	{
		MSG_BOX(TEXT("Failed to Created : CMesh"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMesh::Free()
{
	__super::Free();

}
