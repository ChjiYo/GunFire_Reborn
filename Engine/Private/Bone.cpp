#include "..\Public\Bone.h"

CBone::CBone()
{

}

HRESULT CBone::Initialize(const aiNode * pAINode, _int iParentIndex)
{
	strcpy_s(m_szName, pAINode->mName.data);

	m_iParentIndex = iParentIndex;

	/* assimp에서 직접 로드한 행렬의 경우, 전치되어있다. */
	memcpy(&m_TransformationMatrix, &pAINode->mTransformation, sizeof(_float4x4));

	/* TransformationMatrix : 상황에 맞게 업데이트 해주는 작업이 필요하다. */
	/* 추후, 애니메이션 정보(뼈의 시간에 따른 상태(m_TransformationMatrix))를 로드하게되면 */
	/* 특정 애니메이션을 구동한다 == 이 애니메이션에서 사용하고 있는 뼈들의 m_TransformationMatrix을 갱신해준다. */
	XMStoreFloat4x4(&m_TransformationMatrix, XMMatrixTranspose(XMLoadFloat4x4(&m_TransformationMatrix)));

	/* 초기 상태는 항등이지만, 추후 CombinedTransformationMAtrix를 갱신해주는 작업을 수행해야한다. */
	XMStoreFloat4x4(&m_CombinedTransformationMatrix, XMMatrixIdentity());
	
	return S_OK;
}

void CBone::Update_CombinedTransformationMatrix(const vector<CBone*>& Bones, _fmatrix PreTransformMatrix)
{
	/* 1. 특정 애니메이션을 내가 재생했다면. == 이 애니메이션이 구동하는 뼈의 TransformationMatrix가 갱신됐다. */
	/* 2. 모든 뼈들을 순회하면서 갱신된 특정 뼈의 TransformationMatrix를 포함한 CombinedTransformation을 갱신해준다. */
	// m_CombinedTransformationMatrix = m_TransformationMatrix * 부모`s CombinedTransformationMatrix;

	if (-1 == m_iParentIndex)
		XMStoreFloat4x4(&m_CombinedTransformationMatrix, PreTransformMatrix * XMLoadFloat4x4(&m_TransformationMatrix));

	else
		XMStoreFloat4x4(&m_CombinedTransformationMatrix,
			XMLoadFloat4x4(&m_TransformationMatrix) * XMLoadFloat4x4(&Bones[m_iParentIndex]->m_CombinedTransformationMatrix));	
}

CBone * CBone::Create(const aiNode * pAINode, _int iParentIndex)
{
	CBone*		pInstance = new CBone();

	if (FAILED(pInstance->Initialize(pAINode, iParentIndex)))
	{
		MSG_BOX(TEXT("Failed to Created : CBone"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CBone * CBone::Clone()
{
	return new CBone(*this);
}

void CBone::Free()
{
	__super::Free();

}

