#include "EnemyCollider.h"
#include <utility/CollisionUtility.h>
#include <type/ColliderTypeID.h>
#include <component/HPComponent.h>

void EnemyCollider::OnCollisionEnter(Collider* other)
{
    ColliderTypeID otherID = static_cast<ColliderTypeID>(other->GetTypeID());
    if (otherID == ColliderTypeID::Player || otherID == ColliderTypeID::Terrain)
    {
        // プレイヤーに当たったら一旦押し戻しを行う
        Tako::Vector3 pushback = CollisionUtility::CalcPushback(this, other);
        // エネミーに押し戻しのコールバックが設定されていれば呼び出す
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

void EnemyCollider::OnCollisionStay(Collider* other)
{
    ColliderTypeID otherID = static_cast<ColliderTypeID>(other->GetTypeID());
    if (otherID == ColliderTypeID::Player || otherID == ColliderTypeID::Terrain)
    {
        // プレイヤーに当たったら一旦押し戻しを行う
        Tako::Vector3 pushback = CollisionUtility::CalcPushback(this, other);
        // エネミーに押し戻しのコールバックが設定されていれば呼び出す
        if (pushBackCallback_)
        {
            pushBackCallback_(pushback);
        }
    }
}

void EnemyCollider::OnCollisionExit(Collider* other)
{

}