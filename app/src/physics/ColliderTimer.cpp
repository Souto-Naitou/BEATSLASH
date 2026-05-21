#include "ColliderTimer.h"
#include <FrameTimer.h>



void ColliderTimer::Enable(float activeTime)
{
    activeTime_ = activeTime;
    isActive_ = true;
}

void ColliderTimer::UpdateTime()
{
    elapsedTime_ += Tako::FrameTimer::GetInstance()->GetDeltaTime();
    if (elapsedTime_ >= activeTime_)
    {
        elapsedTime_ = 0.0f; // タイマーをリセット
        isActive_ = false;
    }
}

void ColliderTimer::UpdateColliderState(Tako::Collider* pCollier)
{
    pCollier->SetActive(isActive_);
}
