#pragma once
#include <OBBCollider.h>
#include <character/player/Player.h>
#include <character/player/PlayerAttackHitReceiver.h>

#include <unordered_set>

class ComboBuffSystem;

class PlayerAttackCollider : public Tako::OBBCollider
{
public:
    PlayerAttackCollider(PlayerAttackHitReceiver& hitReceiver)
        : hitReceiver_(hitReceiver), attackId_(nextAttackId_++) {}

    void OnCollisionEnter(Collider* other) override;


    void OnCollisionStay(Collider* other) override;


    void OnCollisionExit(Collider* other) override;

    // この攻撃（1スイング）の一意ID。被弾側の多段ヒット判定に使う
    uint32_t GetAttackId() const { return attackId_; }

private:
    static inline uint32_t nextAttackId_ = 0;

    PlayerAttackHitReceiver& hitReceiver_;
    uint32_t attackId_;
    std::unordered_set<Collider*> hitTargets_;  // この攻撃が既にヒットさせた相手
};
