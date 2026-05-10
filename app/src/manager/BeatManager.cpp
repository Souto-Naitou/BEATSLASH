#include "BeatManager.h"

#include <ozSound/audio/SoundEngine.h>
#include <ozSound/audio/AudioSystem.h>
#include <cmath>

using namespace ozSound;

BeatManager::BeatManager() :
    bpm_(120.0f),
    offset_(0.0f),
    lastBeat_(0),
    playing_(false),
    volume_(0.5f),
    soundEnabled_(true)
{
}

void BeatManager::Initialize(float bpm, float offset, const std::string& soundPath)
{
    bpm_ = bpm;
    offset_ = offset;
    lastBeat_ = -1;
    playing_ = false;

    //// サウンドを読み込む
    //if (soundEnabled_)
    //{
    //    soundInstance_ = ozSound::AudioSystem::GetInstance()->Load(soundPath);
    //}
}

void BeatManager::Update()
{
    // 再生中か確認
    if (!playing_) return;
    //if (!musicVoiceInstance_ || !musicVoiceInstance_->IsPlaying())return;

    // 新しい拍かチェック
    if (IsNewBeat() && soundEnabled_)
    {
        metornomeHandle_ = SoundEngine::GetInstance()->Play("Metronome", volume_, false, 0.0f);
    }
}

void BeatManager::Start()
{
    if (!playing_)
    {
        playing_ = true;
    }
}

void BeatManager::Stop()
{
    if (playing_)
    {
        playing_ = false;

        // 音を停止
        if (soundEnabled_ && IsValid())
        {
            SoundEngine::GetInstance()->Stop(musicHandle_);
        }
    }
}

void BeatManager::Reset()
{
    // 拍数のリセット
    lastBeat_ = 0;

    // 音を停止
    if (soundEnabled_ && IsValid())
    {
        SoundEngine::GetInstance()->Stop(musicHandle_);
    }
}

float BeatManager::GetCurrentBeat() const
{
    if (!IsValid())
        return 0.0f;

    float currentTime = SoundEngine::GetInstance()->GetElapsedTime(musicHandle_) - offset_;
    return currentTime / GetSecondsPerBeat();
}

int BeatManager::GetNearestBeat() const
{
    return static_cast<int>(std::floor(GetCurrentBeat()));
}

bool BeatManager::IsNewBeat()
{
    int currentBeat = GetNearestBeat();
    if (currentBeat > lastBeat_)
    {
        lastBeat_ = currentBeat;
        return true;
    }
    return false;
}

bool BeatManager::IsBeatTriggered(float tolerance) const
{
    float currentBeat = GetCurrentBeat();
    float fractionalPart = currentBeat - std::floor(currentBeat);

    // 拍のタイミング内かをチェック
    return (fractionalPart < tolerance || fractionalPart >(1.0f - tolerance));
}

void BeatManager::SetBPM(float bpm)
{
    if (bpm <= 0.0f) return;
    bpm_ = bpm;
    Reset();
}

void BeatManager::SetMusicSoundHandle(SoundHandle soundHandle)
{
    musicHandle_ = soundHandle;
}

bool BeatManager::IsValid() const
{
    if (musicHandle_ == (std::numeric_limits<uint32_t>::max)())
        return false;
    if (bpm_ <= 0.0f)
        return false;

    return true;
}
