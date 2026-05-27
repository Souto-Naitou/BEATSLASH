#include "PlayerAttackCollider.h"

#include <combo/ComboBuffSystem.h>
#include <type/ColliderTypeID.h>

void PlayerAttackCollider::OnCollisionEnter(Collider* other)
{
    auto ID = static_cast<ColliderTypeID>(other->GetTypeID());

    if (ID == ColliderTypeID::Enemy)
    {
        hitReceiver_.ReceiveHit(PlayerAttackHitReceiver::HitInfo());
    }
}

void PlayerAttackCollider::OnCollisionStay(Collider* other)
{
}

void PlayerAttackCollider::OnCollisionExit(Collider* other)
{
}
