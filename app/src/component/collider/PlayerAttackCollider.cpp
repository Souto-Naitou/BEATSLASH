#include "PlayerAttackCollider.h"

#include <combo/ComboBuffSystem.h>
#include <type/ColliderTypeID.h>

void PlayerAttackCollider::OnCollisionEnter(Collider* other)
{
    auto ID = static_cast<ColliderTypeID>(other->GetTypeID());
    
    if (ID == ColliderTypeID::Enemy || ID == ColliderTypeID::Boss)
    {
        // 横振りでコライダーが出入りしてもスイング中は同一相手に1回だけヒットさせる
        if (!hitTargets_.insert(other).second) return;

        PlayerAttackHitReceiver::HitInfo info;
        info.position = other->GetCenter();
        hitReceiver_.ReceiveHit(info);
    }
}

void PlayerAttackCollider::OnCollisionStay(Collider* other)
{
}

void PlayerAttackCollider::OnCollisionExit(Collider* other)
{
}
