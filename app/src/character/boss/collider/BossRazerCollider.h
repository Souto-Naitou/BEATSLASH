#pragma once
#include <OBBCollider.h>

/// <summary>
/// ボスのレーザー攻撃用コライダー
/// プレイヤーへのダメージ適用はPlayerCollider側のBossAttack分岐で行う
/// </summary>
class BossRazerCollider : public Tako::OBBCollider
{
public:
    void OnCollisionEnter(Collider* other) override;
    void OnCollisionStay(Collider* other) override;
    void OnCollisionExit(Collider* other) override;
};
