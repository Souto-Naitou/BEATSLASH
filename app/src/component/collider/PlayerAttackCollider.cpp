#include "PlayerAttackCollider.h"

#include <combo/ComboBuffSystem.h>
#include <type/ColliderTypeID.h>

void PlayerAttackCollider::OnCollisionEnter(Collider* other)
{
    auto ID = static_cast<ColliderTypeID>(other->GetTypeID());
    
    if (ID == ColliderTypeID::Enemy || ID == ColliderTypeID::Boss)
    {
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
