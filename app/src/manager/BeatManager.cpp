#include "BeatManager.h"

#include <ozSound/audio/SoundEngine.h>
#include <cmath>

using namespace ozSound;

BeatClock::BeatClock()
{
    QueryPerformanceFrequency(&qpcFrequency_);
}

void BeatClock::Initialize(float bpm, float offset)
{
    bpm_      = bpm;
    offset_   = offset;
    lastBeat_ = -1;
    playing_  = false;
}

void BeatClock::Start()
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

void BeatClock::Stop()
{
    if (!playing_) return;
    playing_ = false;

    if (soundEnabled_ && IsHandleValid())
    {
        SoundEngine::GetInstance()->Stop(musicHandle_);
    }
}

void BeatClock::Reset()
{
    // 拍カウンタと内部時刻をリセット (再生状態は維持)
    lastBeat_ = -1;
    QueryPerformanceCounter(&qpcStart_);
}

void BeatClock::Update()
{
    if (!playing_)  return;
    if (!IsValid()) return;

    if (ConsumeBeatIfAdvanced() && soundEnabled_)
    {
        SoundEngine::GetInstance()->Play("Metronome", volume_, false, 0.0f);
    }
}

void BeatClock::SetBPM(float bpm)
{
    if (bpm <= 0.0f) return;
    bpm_ = bpm;
    Reset();
}

void BeatClock::SetMusicSoundHandle(SoundHandle handle)
{
    musicHandle_ = handle;
}

float BeatClock::GetCurrentBeat() const
{
    return static_cast<float>(GetCurrentBeatD());
}

int BeatClock::GetCurrentBeatIndex() const
{
    if (!IsValid()) return 0;
    return static_cast<int>(std::floor(GetCurrentBeatD()));
}

float BeatClock::GetDeltaToNearestBeat() const
{
    if (!IsValid()) return 0.0f;

    double cb = GetCurrentBeatD();
    return static_cast<float>(cb - std::round(cb));
}

float BeatClock::GetDeltaToNearestBeatSeconds() const
{
    if (!IsValid()) return 0.0f;

    double cb = GetCurrentBeatD();
    return static_cast<float>((cb - std::round(cb)) * GetSecondsPerBeatD());
}

double BeatClock::GetElapsedTimeD() const
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

double BeatClock::GetCurrentBeatD() const
{
    if (!IsValid()) return 0.0;

    double currentTime = GetElapsedTimeD() - static_cast<double>(offset_);
    return currentTime / GetSecondsPerBeatD();
}

double BeatClock::ClockToSecondsD(int64_t clock) const
{
    return static_cast<double>(clock) / static_cast<double>(qpcFrequency_.QuadPart);
}

bool BeatClock::IsValid() const
{
    if (bpm_ <= 0.0f) return false;

    switch (countMode_)
    {
        case CountMode::QPC:   return true;             // QPC は常に有効
        case CountMode::Sound: return IsHandleValid();
        default:               return false;
    }
}

bool BeatClock::IsHandleValid() const
{
    return musicHandle_ != (std::numeric_limits<uint32_t>::max)();
}

bool BeatClock::ConsumeBeatIfAdvanced()
{
    int currentIdx = static_cast<int>(std::floor(GetCurrentBeatD()));
    if (currentIdx > lastBeat_)
    {
        lastBeat_ = currentIdx;
        return true;
    }
    return false;
}