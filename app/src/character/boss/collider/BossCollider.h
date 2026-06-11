#pragma once
#include <OBBCollider.h>
#include <functional>

using BossPushBackCallback = std::function<void(const Tako::Vector3& pushback)>;

class HPComponent;

class BossCollider : public Tako::OBBCollider
{
public:
    void OnCollisionEnter(Collider* other) override;
    void OnCollisionStay(Collider* other) override;
    void OnCollisionExit(Collider* other) override;

    void SetPushBackCallback(BossPushBackCallback callback) { pushBackCallback_ = callback; }
    void SetHPComponent(HPComponent* hp) { pHp_ = hp; }

private:
    BossPushBackCallback pushBackCallback_;

    HPComponent* pHp_ = nullptr;
};
