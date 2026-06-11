#pragma once

#include "IPlayerSkill.h"
#include <utility/StopWatch.h>
#include <manager/BeatClock.h>

class UpTempo : public IPlayerSkill
{
public:

    UpTempo(BeatClock& beatClock) : beatClock_(beatClock) {}
    ~UpTempo() override = default;

    void Activate() override;
    void Update() override;
    void End() override;

    void SetMusicSpeedUpRate(float musicSpeedUpRate) { musicSpeedUpRate_ = musicSpeedUpRate; }
    float GetPowerUpRate() const;

    bool IsActive() const override;

private:

    BeatClock& beatClock_;
    StopWatch stopWatch_ = {};

    bool isActive_ = false;

    float duratoin_ = 5.0f;

    float musicSpeedUpRate_ = 1.1f;


};