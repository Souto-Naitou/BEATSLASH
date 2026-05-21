#pragma once
#include <component/collider/PlayerAttackCollider.h>
#include <physics/ColliderTimer.h>
#include <physics/ColliderRepository.h>

class PlayerAttack
{
public:
    PlayerAttack(ColliderRepository& colliderRepository);
    void Update(float deltaTime);

private:
    Tako::Collider* pCollider_      = nullptr;      // 攻撃用コライダー
    ColliderTimer   colliderTimer_  = {};           // コライダーの有効時間管理
};