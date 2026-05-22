#pragma once
#include <Collider.h>

/// コライダーの有効時間を管理するクラス
/// 特定の時間だけコライダーを有効にしたい場合に使用する
class ColliderTimer
{
public:
    void Enable(float activeTime);
    void Update(float deltaTime);

    bool IsActive() const { return isActive_; }

private:
    float           activeTime_     = 0.0f;     // コライダーが有効な時間
    float           elapsedTime_    = 0.0f;     // 経過時間
    bool            isActive_       = false;    // コライダーが有効かどうか
};