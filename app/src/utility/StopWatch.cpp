#include "./StopWatch.h"

StopWatch::StopWatch()
{
    QueryPerformanceFrequency(&mFreq_);
}

void StopWatch::Start()
{
    if (isRunning_) return;

    QueryPerformanceCounter(&mStart_);
    isStart_ = true;
    isRunning_ = true;
}

void StopWatch::Stop()
{
    if (!isRunning_) return;

    GetNow<double>();
    nowBeforeStop_ += now_;
    isRunning_ = false;
}

void StopWatch::Reset()
{
    mStart_ = {};
    now_ = 0.0;
    nowBeforeStop_ = 0.0;
    isStart_ = false;
    isRunning_ = false;
}