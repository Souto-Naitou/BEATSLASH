#pragma once
#include <physics/ColliderTimer.h>
#include <physics/ColliderRepository.h>
#include <debug/GameParameter.h>

#include <Transform.h>
#include <Collider.h>

class ComboBuffSystem;

class PlayerAttack
{
public:
    PlayerAttack(ColliderRepository& colliderRepository, const Tako::Vector3& position, ComboBuffSystem* comboBuffSystem);
    void Update(float deltaTime);

private:
    EnableDebug("PlayerAttack");

    GameParameter(float, kColliderActiveTime_, 0.5f);   // コライダーの有効時間
    GameParameterView(Tako::Transform, transform_, {}); // 攻撃のトランスフォーム（デバッグ表示用）

    Tako::Collider* pCollider_      = nullptr;          // 攻撃用コライダー
    ColliderTimer   colliderTimer_  = {};               // コライダーの有効時間管理
};
