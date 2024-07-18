#pragma once

#include "Base.h"

/* 채널(aiNodeAnim) : 하나의 뼈 정보, 이 뼈가 특정 시간대에 어떤 상태를 띄어야 하는가? */
/* 즉, 채널은 시간에 따른 상태(키프레임)값들을 가진다. */

BEGIN(Engine)

class CChannel final : public CBase
{
public:
	typedef struct
	{
		_char* szName = {};
		_uint iNameLength = {};

		_uint iBoneIndex = {};
		_uint iNumKeyFrames = {};

		vector<KEYFRAME>	KeyFrames = {};
	}CHANNEL_DESC;
private:
	CChannel();
	virtual ~CChannel() = default;

public:
	HRESULT Initialize(const aiNodeAnim* pAIChannel, const vector<class CBone*>& Bones);
	HRESULT Initialize(CHANNEL_DESC Desc, const vector<class CBone*>& Bones);
	void Compute_TransformationMatrix(_float fCurrentPosition, const vector<class CBone*>& Bones, _uint* pKeyFrameIndex);

public:
	CHANNEL_DESC Get_Desc() { return Desc; }

private:
	_char				m_szName[MAX_PATH] = {};

	_uint				m_iBoneIndex = {};
	_uint				m_iNumKeyFrames = {};
	vector<KEYFRAME>	m_KeyFrames;

	CHANNEL_DESC		Desc = {};
	vector<CHANNEL_DESC> vecDesc = {};

public:
	static CChannel* Create(const aiNodeAnim* pAIChannel, const vector<class CBone*>& Bones);
	static CChannel* Create(CHANNEL_DESC Desc, const vector<class CBone*>& Bones);
	virtual void Free() override;
};

END