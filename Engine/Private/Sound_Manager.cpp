#include "..\Public\Sound_Manager.h"

#include "Layer.h"
#include "GameObject.h"
#include <locale>
#include <codecvt>

CSound_Manager::CSound_Manager()
{
}

HRESULT CSound_Manager::Initialize(_uint iNumLevels)
{
    m_iNumLevels = iNumLevels;

    // 동적 배열 할당
    m_soundMap = new map<wstring, FMOD_SOUND*>[iNumLevels];
    m_channelMap = new map<wstring, FMOD_CHANNEL*>[iNumLevels];

    // FMOD 시스템 초기화
    FMOD_System_Create(&m_pSoundSystem, FMOD_VERSION);
    FMOD_System_Init(m_pSoundSystem, MAX_CHANNELS, FMOD_INIT_NORMAL, nullptr);

    // 채널 그룹 생성
    FMOD_System_CreateChannelGroup(m_pSoundSystem, nullptr, &m_pChannelGroup);

    return S_OK;
}

void CSound_Manager::Priority_Update(_float fTimeDelta)
{

}

void CSound_Manager::Update(_float fTimeDelta)
{

}

void CSound_Manager::Late_Update(_float fTimeDelta)
{

}

void CSound_Manager::Register_Sound(const std::wstring& filePath, const std::wstring& alias, _uint levelId, _uint soundType)
{
    if (levelId >= m_iNumLevels) return;

    // 사운드가 이미 등록되어 있는지 확인
    if (m_soundMap[levelId].find(alias) != m_soundMap[levelId].end())
    {
        return; // 이미 존재하므로 등록하지 않고 성공으로 반환
    }

    FMOD_SOUND* sound;

    // wstring을 string으로 변환
    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    std::string filePathStr = converter.to_bytes(filePath);

    if (FMOD_System_CreateSound(m_pSoundSystem, filePathStr.c_str(), FMOD_DEFAULT, nullptr, &sound) != FMOD_OK)
    {
        return;
    }

    // 사운드 등록
    m_soundMap[levelId][alias] = sound;
}

void CSound_Manager::Play_Sound(const std::wstring& alias, _uint levelId, bool loop)
{
    if (levelId >= m_iNumLevels) return;

    auto it = m_soundMap[levelId].find(alias);
    if (it == m_soundMap[levelId].end()) return;

    FMOD_CHANNEL* channel = nullptr;

    FMOD_MODE mode = loop ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF;
    FMOD_Sound_SetMode(it->second, mode);

    // 현재 재생 중인 채널 수를 확인
    int playingChannels = 0;
    FMOD_ChannelGroup_GetNumChannels(m_pChannelGroup, &playingChannels);

    // 재생이 완료된 채널을 해제하는 코드 추가
    for (int i = 0; i < playingChannels; ++i)
    {
        FMOD_CHANNEL* tempChannel = nullptr;
        if (FMOD_ChannelGroup_GetChannel(m_pChannelGroup, i, &tempChannel) == FMOD_OK)
        {
            FMOD_BOOL isPlaying = false;
            if (FMOD_Channel_IsPlaying(tempChannel, &isPlaying) == FMOD_OK && !isPlaying)
            {
                FMOD_Channel_Stop(tempChannel);  // 재생이 완료된 채널을 해제
            }
        }
    }

    // 재생 중인 채널 수를 다시 확인
    FMOD_ChannelGroup_GetNumChannels(m_pChannelGroup, &playingChannels);

    if (playingChannels < MAX_CHANNELS) // MAX_CHANNELS는 사용자가 정의한 최대 채널 수
    {
        // 채널 그룹에 속한 새로운 채널을 생성하여 재생
        if (FMOD_System_PlaySound(m_pSoundSystem, it->second, m_pChannelGroup, false, &channel) == FMOD_OK)
        {
            m_channelMap[levelId][alias] = channel;
        }
    }
    else
    {
        // 모든 채널이 사용 중인 경우, 재생을 실패하게 함
        // 또는 기존 채널을 중단하고 새로운 채널로 교체할 수도 있음
        // 예: FMOD_Channel_Stop(기존채널); FMOD_System_PlaySound(...);
        return;
    }
}

void CSound_Manager::Stop_Sound(const std::wstring& alias, _uint levelId)
{
    if (levelId >= m_iNumLevels) return;

    auto it = m_channelMap[levelId].find(alias);
    if (it == m_channelMap[levelId].end()) return;

    if (FMOD_Channel_Stop(it->second) != FMOD_OK)
    {
        return;
    }
}

void CSound_Manager::Set_Volume(const std::wstring& alias, _uint levelId, float volume)
{
    if (levelId >= m_iNumLevels) return;

    auto it = m_channelMap[levelId].find(alias);
    if (it == m_channelMap[levelId].end()) return;

    if (FMOD_Channel_SetVolume(it->second, volume) != FMOD_OK)
    {
        return;
    }
}

void CSound_Manager::Stop_All_Sounds(_uint levelId)
{
    if (levelId >= m_iNumLevels) return;

    for (auto& pair : m_channelMap[levelId])
    {
        FMOD_Channel_Stop(pair.second);
    }
}

CSound_Manager* CSound_Manager::Create(_uint iNumLevels)
{
    CSound_Manager* pInstance = new CSound_Manager();

    if (FAILED(pInstance->Initialize(iNumLevels)))
    {
        MSG_BOX(TEXT("Failed to Created : CSound_Manager"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CSound_Manager::Free()
{
    for (_uint i = 0; i < m_iNumLevels; ++i)
    {
        for (auto& pair : m_soundMap[i])
        {
            FMOD_Sound_Release(pair.second);
        }
        m_soundMap[i].clear();
        m_channelMap[i].clear();
    }

    Safe_Delete_Array(m_soundMap);
    Safe_Delete_Array(m_channelMap);

    if (m_pSoundSystem)
    {
        FMOD_System_Close(m_pSoundSystem);
        FMOD_System_Release(m_pSoundSystem);
        m_pSoundSystem = nullptr;
    }

    __super::Free();
}
