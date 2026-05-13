#include "BeatManager.h"

#include <ozSound/audio/SoundEngine.h>
#include <cmath>

using namespace ozSound;

BeatManager::BeatManager()
{
    QueryPerformanceFrequency(&qpcFrequency_);
}

void BeatManager::Initialize(float bpm, float offset)
{
    bpm_      = bpm;
    offset_   = offset;
    lastBeat_ = -1;
    playing_  = false;
}

void BeatManager::Start()
{
    if (playing_) return;

    // ハンドルが事前に設定されていれば Sound モード、そうでなければ QPC モード
    countMode_ = IsHandleValid() ? CountMode::Sound : CountMode::QPC;

    if (countMode_ == CountMode::QPC)
    {
        QueryPerformanceCounter(&qpcStart_);
    }

    lastBeat_ = -1;
    playing_  = true;
}

void BeatManager::Stop()
{
    if (!playing_) return;
    playing_ = false;

    if (soundEnabled_ && IsHandleValid())
    {
        SoundEngine::GetInstance()->Stop(musicHandle_);
    }
}

void BeatManager::Reset()
{
    // 拍カウンタと内部時刻をリセット (再生状態は維持)
    lastBeat_ = -1;
    QueryPerformanceCounter(&qpcStart_);
}

void BeatManager::Update()
{
    if (!playing_)  return;
    if (!IsValid()) return;

    if (ConsumeBeatIfAdvanced() && soundEnabled_)
    {
        SoundEngine::GetInstance()->Play("Metronome", volume_, false, 0.0f);
    }
}

void BeatManager::SetBPM(float bpm)
{
    if (bpm <= 0.0f) return;
    bpm_ = bpm;
    Reset();
}

void BeatManager::SetMusicSoundHandle(SoundHandle handle)
{
    musicHandle_ = handle;
}

float BeatManager::GetCurrentBeat() const
{
    return static_cast<float>(GetCurrentBeatD());
}

int BeatManager::GetCurrentBeatIndex() const
{
    if (!IsValid()) return 0;
    return static_cast<int>(std::floor(GetCurrentBeatD()));
}

float BeatManager::GetDeltaToNearestBeat() const
{
    if (!IsValid()) return 0.0f;

    double cb = GetCurrentBeatD();
    return static_cast<float>(cb - std::round(cb));
}

float BeatManager::GetDeltaToNearestBeatSeconds() const
{
    if (!IsValid()) return 0.0f;

    double cb = GetCurrentBeatD();
    return static_cast<float>((cb - std::round(cb)) * GetSecondsPerBeatD());
}

double BeatManager::GetElapsedTimeD() const
{
    switch (countMode_)
    {
        case CountMode::QPC:
        {
            LARGE_INTEGER now;
            QueryPerformanceCounter(&now);
            return ClockToSecondsD(now.QuadPart - qpcStart_.QuadPart);
        }

        case CountMode::Sound:
            return static_cast<double>(
                SoundEngine::GetInstance()->GetElapsedTime(musicHandle_));

        default:
            return 0.0;
    }
}

double BeatManager::GetCurrentBeatD() const
{
    if (!IsValid()) return 0.0;

    double currentTime = GetElapsedTimeD() - static_cast<double>(offset_);
    return currentTime / GetSecondsPerBeatD();
}

double BeatManager::ClockToSecondsD(int64_t clock) const
{
    return static_cast<double>(clock) / static_cast<double>(qpcFrequency_.QuadPart);
}

bool BeatManager::IsValid() const
{
    if (bpm_ <= 0.0f) return false;

    switch (countMode_)
    {
        case CountMode::QPC:   return true;             // QPC は常に有効
        case CountMode::Sound: return IsHandleValid();
        default:               return false;
    }
}

bool BeatManager::IsHandleValid() const
{
    return musicHandle_ != (std::numeric_limits<uint32_t>::max)();
}

bool BeatManager::ConsumeBeatIfAdvanced()
{
    int currentIdx = static_cast<int>(std::floor(GetCurrentBeatD()));
    if (currentIdx > lastBeat_)
    {
        lastBeat_ = currentIdx;
        return true;
    }
    return false;
}