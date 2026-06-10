#include "BossCollider.h"
#include <utility/CollisionUtility.h>
#include <type/ColliderTypeID.h>
#include <component/HPComponent.h>

void BossCollider::OnCollisionEnter(Collider* other)
{
    ColliderTypeID otherID = static_cast<ColliderTypeID>(other->GetTypeID());
    if (otherID == ColliderTypeID::Player || otherID == ColliderTypeID::Terrain || otherID == ColliderTypeID::Enemy)
    {
        // 当たったら一旦押し戻しを行う
        Tako::Vector3 pushback = CollisionUtility::CalcPushback(this, other);
        if (pushBackCallback_)
        {
            pushBackCallback_(pushback);
        }
    }
    else if (otherID == ColliderTypeID::PlayerAttack)
    {
        if (pHp_)
            pHp_->Damage(25); // TODO : 仮のダメージ値
    }
}

void BossCollider::OnCollisionStay(Collider* other)
{
    ColliderTypeID otherID = static_cast<ColliderTypeID>(other->GetTypeID());
    if (otherID == ColliderTypeID::Player || otherID == ColliderTypeID::Terrain || otherID == ColliderTypeID::Enemy)
    {
        // 当たったら一旦押し戻しを行う
        Tako::Vector3 pushback = CollisionUtility::CalcPushback(this, other);
        if (pushBackCallback_)
        {
            pushBackCallback_(pushback);
        }
    }
}

void BossCollider::OnCollisionExit(Collider* other)
{

}
