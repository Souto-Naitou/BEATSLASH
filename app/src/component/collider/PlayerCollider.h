#pragma once

#include <OBBCollider.h>
#include <functional>

using PlayerPushBackCallback = std::function<void(const Tako::Vector3& pushback)>;

class ComboBuffSystem;

class PlayerCollider : public Tako::OBBCollider
{
public:

    void OnCollisionEnter(Collider* other) override;


    void OnCollisionStay(Collider* other) override;


    void OnCollisionExit(Collider* other) override;


    void SetPushBackCallback(PlayerPushBackCallback callback) { pushBackCallback_ = callback; }
    void SetComboSystem(ComboBuffSystem* comboBuffSystem) { pComboBuffSystem_ = comboBuffSystem; }
private:

    PlayerPushBackCallback pushBackCallback_;
    ComboBuffSystem* pComboBuffSystem_ = nullptr; // コンボシステムの参照
};