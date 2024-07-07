#include "..\Public\Animation.h"
#include "Channel.h"

CAnimation::CAnimation()
{
}

CAnimation::CAnimation(const CAnimation & Prototype)
{
}

HRESULT CAnimation::Initialize(const aiAnimation * pAIAnimation, const vector<class CBone*>& Bones, vector<_uint>& KeyFrameIndices)
{
	strcpy_s(m_szName, pAIAnimation->mName.data);

	/* 애니메이션 재생하는데 걸리는 총 거리. */
	m_fDuration = pAIAnimation->mDuration;

	/* 애니메이션 재생바초당 얼마나 이동해야하는지(재생속도) */
	m_fTickPerSecond = pAIAnimation->mTicksPerSecond;

	/* 애니메이션이 컨트롤하는 뼈의 갯수 /*/
	m_iNumChannels = pAIAnimation->mNumChannels;

	KeyFrameIndices.resize(m_iNumChannels);

	for (size_t i = 0; i < m_iNumChannels; i++)
	{
		CChannel*		pChannel = CChannel::Create(pAIAnimation->mChannels[i], Bones);
		if (nullptr == pChannel)
			return E_FAIL;

		m_Channels.emplace_back(pChannel);
	}

	return S_OK;
}

void CAnimation::Update_TransformationMatrix(_float* pCurrentAnimPosition, _float fTimeDelta, const vector<class CBone*>& Bones, _bool isLoop, vector<_uint>& KeyFrameIndices)
{
	/* 현재 재생위치를 계싼하낟. */
	*pCurrentAnimPosition += m_fTickPerSecond * fTimeDelta;

	if (*pCurrentAnimPosition >= m_fDuration)
	{
		*pCurrentAnimPosition = m_fDuration;

		if (true == isLoop)
		{
			*pCurrentAnimPosition = 0.f;
		}
	
	}

	for (size_t i = 0; i < m_iNumChannels; i++)
	{
		m_Channels[i]->Compute_TransformationMatrix(*pCurrentAnimPosition, Bones, &KeyFrameIndices[i]);
	}
}

CAnimation * CAnimation::Create(const aiAnimation * pAIAnimation, const vector<class CBone*>& Bones, vector<_uint>& KeyFrameIndices)
{
	CAnimation*		pInstance = new CAnimation();

	if (FAILED(pInstance->Initialize(pAIAnimation, Bones, KeyFrameIndices)))
	{
		MSG_BOX(TEXT("Failed to Created : CAnimation"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CAnimation::Free()
{
	__super::Free();

}
