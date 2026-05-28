#pragma once
#include <physics/ColliderTimer.h>
#include <physics/ColliderRepository.h>
#include <character/player/PlayerAttackHitReceiver.h>
#include <debug/GameParameter.h>

#include <Transform.h>
#include <Collider.h>
#include <Vector3.h>

class ComboBuffSystem;

class PlayerAttack
{
public:
    PlayerAttack(ColliderRepository& colliderRepository, PlayerAttackHitReceiver& hitReceiver, const Tako::Vector3& position);
    void Update(float deltaTime);
    bool IsActive() const { return pCollider_ != nullptr; }

private:
    EnableDebug("PlayerAttack");

    float           kColliderActiveTime_    = 0.5f;     // コライダーの有効時間
    Tako::Transform transform_              = {};       // 攻撃のトランスフォーム（デバッグ表示用）

    Tako::Collider* pCollider_              = nullptr;  // 攻撃用コライダー
    ColliderTimer   colliderTimer_          = {};       // コライダーの有効時間管理
};
