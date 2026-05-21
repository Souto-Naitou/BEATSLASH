#pragma once
#include <Collider.h>

class ColliderTimer
{
public:
    void Enable(float activeTime);
    void UpdateTime();
    void UpdateColliderState(Tako::Collider* pCollier);

    bool IsActive() const { return isActive_; }

private:
    float           activeTime_     = 0.0f;     // コライダーが有効な時間
    float           elapsedTime_    = 0.0f;     // 経過時間
    bool            isActive_       = false;
};