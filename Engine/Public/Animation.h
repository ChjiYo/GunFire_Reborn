#pragma once

#include "Base.h"

/* 애니메이션 : Attack, Idle, Walk, Jump */
/* 애니메이션 클래스의 기능 */ 
/* 여러개의 애니메이션(Attack, Idle, Walk, Jump )중, 한 동작(특정애니메이션(Attack)의 시작부터 끝까지)을 표현하기위해 어떤 뼈(채널)들을 움직여야하는지에 대한 정보를 가지낟. */
/* 구현하고자한 동작의 전체 재생 시간.(m_fDuration) */
/* 재생 속도 m_fTickPerSecond */

BEGIN(Engine)

class CAnimation final : public CBase
{
private:
	CAnimation();
	CAnimation(const CAnimation& Prototype);
	virtual ~CAnimation() = default;

public:
	HRESULT Initialize(const aiAnimation* pAIAnimation, const vector<class CBone*>& Bones, vector<_uint>& KeyFrameIndices);
	void Update_TransformationMatrix(_float* pCurrentAnimPosition, _float fTimeDelta, const vector<class CBone*>& Bones, _bool isLoop, vector<_uint>& KeyFrameIndices);

private:
	_char			m_szName[MAX_PATH] = {};
	_float			m_fDuration = {};
	_float			m_fTickPerSecond = {};
	_float			m_fCurrentPosition = {};

private:
	/* 이 애니메이션을 표현하기위해 컨트롤해야하는 뼈의 갯수 */
	_uint						m_iNumChannels = {};

	/* 이 애니메이션을 표현하기위해 컨트롤해야하는 뼈들 */
	vector<class CChannel*>		m_Channels;

public:
	static CAnimation* Create(const aiAnimation* pAIAnimation, const vector<class CBone*>& Bones, vector<_uint>& KeyFrameIndices);
	
	virtual void Free() override;
};

END