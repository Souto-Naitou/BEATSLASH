#include "InputTimingJudge.h"

#include <cmath>

void InputTimingJudge::Initialize(float bpm, float perfectWindow, float goodWindow)
{
    bpm_ = bpm;
    perfectWindow_ = perfectWindow;
    goodWindow_ = goodWindow;

    perfectWindowSec_ = CalculateWindowSec(perfectWindow_);
    goodWindowSec_ = CalculateWindowSec(goodWindow_);
}

JudgeResult InputTimingJudge::Evaluate(float timingDiff) const
{
    float absTimingDiff = std::abs(timingDiff);
    if (absTimingDiff <= perfectWindowSec_ * 0.5f)
    {
        return JudgeResult::Perfect;
    }
    else if(absTimingDiff <= goodWindowSec_ * 0.5f)
    {
        return JudgeResult::Good;
    }
    else
    {
        return JudgeResult::Miss;
    }
}

float InputTimingJudge::GetSecPerBeat() const
{
    return 60.0f / bpm_;
}

float InputTimingJudge::CalculateWindowSec(float window) const
{
    return GetSecPerBeat() * window / 2.0f;
}

