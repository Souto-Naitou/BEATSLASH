#include "ColliderTimer.h"
#include <FrameTimer.h>



void ColliderTimer::Enable(float activeTime)
{
    activeTime_ = activeTime;
    pCollider_->SetActive(true);
}

void ColliderTimer::UpdateTime()
{
    elapsedTime_ += Tako::FrameTimer::GetInstance()->GetDeltaTime();
}
