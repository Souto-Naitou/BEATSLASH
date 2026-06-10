#include "PlayerCollider.h"
#include <utility/CollisionUtility.h>

#include <type/ColliderTypeID.h>
#include <combo/ComboBuffSystem.h>
#include <component/HPComponent.h>

using namespace Tako;

void PlayerCollider::OnCollisionEnter(Collider* other)
{
    ColliderTypeID otherID = static_cast<ColliderTypeID>(other->GetTypeID());

    if(otherID == ColliderTypeID::Terrain)
    {
        Vector3 pushback = CollisionUtility::CalcPushback(this, other);

        if (pushBackCallback_)
            pushBackCallback_(pushback);
    }
    else if(otherID == ColliderTypeID::EnemyAttack)
    {
        if(pComboBuffSystem_)
        {
            pComboBuffSystem_->OnDamaged();
        }
        if (pHPComponent_)
        {
            pHPComponent_->Damage(10); // 仮のダメージ量
        }
    }

}

void PlayerCollider::OnCollisionStay(Collider* other)
{
    ColliderTypeID otherID = static_cast<ColliderTypeID>(other->GetTypeID());

    if (otherID == ColliderTypeID::Terrain)
    {
        Vector3 pushback = CollisionUtility::CalcPushback(this, other);

        if (pushBackCallback_)
            pushBackCallback_(pushback);
    }
}

void PlayerCollider::OnCollisionExit(Collider* other)
{
}
