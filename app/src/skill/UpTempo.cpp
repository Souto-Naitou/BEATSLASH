#include "UpTempo.h"

#include <ozSound/audio/SoundEngine.h>

void UpTempo::Activate()
{
    isActive_ = true;

    // SetPlaySpeedはaudio 経過時間がn倍速で進む
    // elapsedTimeはバッファの進行に依存するため、BPＭの変更は不要
    //beatClock_.SetBPM(originalBPM_* musicSpeedUpRate_);
    ozSound::SoundHandle musicHandle = beatClock_.GetMusicSoundHandle();
    ozSound::SoundEngine::GetInstance()->SetPlaySpeed(musicHandle, musicSpeedUpRate_);

    stopWatch_.Reset();
    stopWatch_.Start();

}

void UpTempo::Update()
{
    if (!isActive_)
        return;

    float elasedTime = stopWatch_.GetNow<float>();
    if (elasedTime >= duratoin_)
    {
        End();
    }
}

void UpTempo::End()
{
    isActive_ = false;
    stopWatch_.Stop();

    //beatClock_.SetBPM(originalBPM_);
    ozSound::SoundHandle musicHandle = beatClock_.GetMusicSoundHandle();
    ozSound::SoundEngine::GetInstance()->SetPlaySpeed(musicHandle, 1.0f);

}

float UpTempo::GetPowerUpRate() const
{
    return musicSpeedUpRate_;
}

bool UpTempo::IsActive() const
{
    return isActive_;
}
