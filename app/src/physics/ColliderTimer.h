#pragma once
#include <Collider.h>

class ColliderTimer
{
public:
    ColliderTimer(Tako::Collider* pCollider) : pCollider_(pCollider) {}
    void Enable(float activeTime);
    void UpdateTime();

private:
    float           activeTime_     = 0.0f;     // コライダーが有効な時間
    float           elapsedTime_    = 0.0f;     // コライダーが生成されてからの経過時間
    Tako::Collider* pCollider_      = nullptr;
};