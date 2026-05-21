#pragma once

#include "JudgeResutl.h"

class InputTimingJudge
{
public:

    void Initialize(float bpm, float perfectWindow, float goodWindow);

    JudgeResult Evaluate(float timingDiff) const;

private:

    float GetSecPerBeat() const;

    float CalculateWindowSec(float window) const;

private:

    // BPMによって変動する閾値

    // 一拍 * 閾値 = 判定窓 sec(± 判定窓/2 sec)
    float perfectWindow_ = 0.2f;    // <- BPM120のとき 0.5*0.2/2 = ±0.0375s(約6フレーム) が perfect
    float goodWindow_ = 0.4f;       // <- BPM120のとき 0.5*0.4/2 = ±0.125s(約15フレーム) が good

    float perfectWindowSec_ = 0.0f;
    float goodWindowSec_ = 0.0f;

    float bpm_ = 120.0f;            // BPM (Beats Per Minute)
};