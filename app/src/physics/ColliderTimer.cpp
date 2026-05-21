#include "ColliderTimer.h"



void ColliderTimer::Enable(float activeTime)
{
    activeTime_ = activeTime;
    isActive_ = true;
}

void ColliderTimer::Update(float deltaTime)
{
    if (!isActive_)
    {
        return;
    }

    elapsedTime_ += deltaTime;

    if (elapsedTime_ >= activeTime_)
    {
        elapsedTime_ = 0.0f; // タイマーをリセット
        isActive_ = false;
    }
}
