#include "..\Public\Channel.h"

#include "Bone.h"

CChannel::CChannel()
{
}

HRESULT CChannel::Initialize(const aiNodeAnim * pAIChannel, const vector<CBone*>& Bones)
{
	/* 이 뼈의 이름은 모델으 전체뼈의 이름과 동기화되어있어요. */
	strcpy_s(m_szName, pAIChannel->mNodeName.data);

	Desc.szName = m_szName;

	/* 추후 계산한 채널의 상태 행렬을 모델 전체 기준 뼈에 빠르게 전달해주기위해 미리 검색하여 인덱스르 ㄹ저앙햏ㄹ운다. */
	auto	iter = find_if(Bones.begin(), Bones.end(), [&](CBone* pBone) {
		if (false == strcmp(m_szName, pBone->Get_Name()))
			return true;
		++m_iBoneIndex;
		return false;
	});	

	Desc.iBoneIndex = m_iBoneIndex;;

	m_iNumKeyFrames = max(pAIChannel->mNumScalingKeys, pAIChannel->mNumRotationKeys);
	m_iNumKeyFrames = max(m_iNumKeyFrames, pAIChannel->mNumPositionKeys);

	Desc.iNumKeyFrames = m_iNumKeyFrames;

	_float3			vScale{};
	_float4			vRotation{};
	_float3			vPosition{};

	/* 키프레임(상태) */
	for (size_t i = 0; i < m_iNumKeyFrames; i++)
	{
		KEYFRAME	KeyFrame{};

		if (i < pAIChannel->mNumScalingKeys)
		{
			memcpy(&vScale, &pAIChannel->mScalingKeys[i].mValue, sizeof(_float3));
			KeyFrame.fTime = pAIChannel->mScalingKeys[i].mTime;
		}
		if (i < pAIChannel->mNumRotationKeys)
		{
			vRotation.x = pAIChannel->mRotationKeys[i].mValue.x;
			vRotation.y = pAIChannel->mRotationKeys[i].mValue.y;
			vRotation.z = pAIChannel->mRotationKeys[i].mValue.z;
			vRotation.w = pAIChannel->mRotationKeys[i].mValue.w;
			KeyFrame.fTime = pAIChannel->mRotationKeys[i].mTime;
		}
		if (i < pAIChannel->mNumPositionKeys)
		{
			memcpy(&vPosition, &pAIChannel->mPositionKeys[i].mValue, sizeof(_float3));
			KeyFrame.fTime = pAIChannel->mPositionKeys[i].mTime;
		}

		KeyFrame.vScale = vScale;
		KeyFrame.vRotation = vRotation;
		KeyFrame.vPosition = vPosition;
		
		m_KeyFrames.emplace_back(KeyFrame);	
	}
	Desc.KeyFrames = m_KeyFrames;

	return S_OK;
}

HRESULT CChannel::Initialize(CHANNEL_DESC Desc, const vector<class CBone*>& Bones)
{
	/* 이 뼈의 이름은 모델으 전체뼈의 이름과 동기화되어있어요. */
	strcpy_s(m_szName, Desc.szName);

	/* 추후 계산한 채널의 상태 행렬을 모델 전체 기준 뼈에 빠르게 전달해주기위해 미리 검색하여 인덱스르 ㄹ저앙햏ㄹ운다. */
	m_iNumKeyFrames = Desc.iNumKeyFrames;
	m_iBoneIndex = Desc.iBoneIndex ;
	m_KeyFrames = Desc.KeyFrames;

	return S_OK;
}

void CChannel::Compute_TransformationMatrix(_float fCurrentPosition, const vector<CBone*>& Bones, _uint* pKeyFrameIndex)
{
	if (0.f == fCurrentPosition)
		(*pKeyFrameIndex)= 0;

	_matrix		TransformationMatrix = XMMatrixIdentity();

	KEYFRAME	LastKeyFrame = m_KeyFrames.back();

	_vector		vScale{}, vRotation{}, vPosition{};

	/* 마지막 키프레임의 상태를 가지낟. */
	if (fCurrentPosition >= LastKeyFrame.fTime)
	{
		vScale = XMLoadFloat3(&LastKeyFrame.vScale);
		vRotation = XMLoadFloat4(&LastKeyFrame.vRotation);
		vPosition = XMVectorSetW(XMLoadFloat3(&LastKeyFrame.vPosition), 1.f);
	}
	/* 왼쪽키프레임와 오른쪽 키프레임사이의 보간된 값을 구하낟. */
	else
	{
		while (fCurrentPosition >= m_KeyFrames[(*pKeyFrameIndex)+ 1].fTime)
			++(*pKeyFrameIndex);

		_float		fRatio = (fCurrentPosition - m_KeyFrames[(*pKeyFrameIndex)].fTime) /
			(m_KeyFrames[(*pKeyFrameIndex)+ 1].fTime - m_KeyFrames[(*pKeyFrameIndex)].fTime);

		_vector		vSourScale, vDestScale;
		_vector		vSourRotation, vDestRotation;
		_vector		vSourPosition, vDestPosition;

		vSourScale = XMLoadFloat3(&m_KeyFrames[(*pKeyFrameIndex)].vScale);
		vSourRotation = XMLoadFloat4(&m_KeyFrames[(*pKeyFrameIndex)].vRotation);
		vSourPosition = XMVectorSetW(XMLoadFloat3(&m_KeyFrames[(*pKeyFrameIndex)].vPosition), 1.f);

		vDestScale = XMLoadFloat3(&m_KeyFrames[(*pKeyFrameIndex)+ 1].vScale);
		vDestRotation = XMLoadFloat4(&m_KeyFrames[(*pKeyFrameIndex)+ 1].vRotation);
		vDestPosition = XMVectorSetW(XMLoadFloat3(&m_KeyFrames[(*pKeyFrameIndex)+ 1].vPosition), 1.f);
		

		vScale = XMVectorLerp(vSourScale, vDestScale, fRatio);
		vRotation = XMQuaternionSlerp(vSourRotation, vDestRotation, fRatio);
		vPosition = XMVectorLerp(vSourPosition, vDestPosition, fRatio);		
	}

	TransformationMatrix = XMMatrixAffineTransformation(vScale, XMVectorSet(0.f, 0.f, 0.f, 1.f), vRotation, vPosition);

	Bones[m_iBoneIndex]->SetUp_TransformationMatrix(TransformationMatrix);


}

CChannel * CChannel::Create(const aiNodeAnim * pAIChannel, const vector<CBone*>& Bones)
{
	CChannel*		pInstance = new CChannel();

	if (FAILED(pInstance->Initialize(pAIChannel, Bones)))
	{
		MSG_BOX(TEXT("Failed to Created : CChannel"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CChannel* CChannel::Create(CHANNEL_DESC Desc, const vector<class CBone*>& Bones)
{
	CChannel* pInstance = new CChannel();

	if (FAILED(pInstance->Initialize(Desc, Bones)))
	{
		MSG_BOX(TEXT("Failed to Created : CChannel"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CChannel::Free()
{
	__super::Free();

}

