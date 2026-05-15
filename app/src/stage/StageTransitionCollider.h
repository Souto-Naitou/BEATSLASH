#pragma once

#include <AABBCollider.h>

#include <functional>

class StageTransitionCollider : public Tako::AABBCollider
{
public:

    void OnCollisionEnter(Tako::Collider* other) override;

    void SetOnTransitionTrigger(const std::function<void()>& callback) { onTransitionTrigger_ = callback; }

private:

    std::function<void()> onTransitionTrigger_; // 遷移開始時のコールバック関数

};
