#include "BossCollider.h"
#include <utility/CollisionUtility.h>
#include <type/ColliderTypeID.h>
#include <component/HPComponent.h>
#include <component/collider/PlayerAttackCollider.h>
#include <ozSound/audio/SoundEngine.h>

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
        // 同じスイングからの多段ヒット防止
        auto* attack = static_cast<PlayerAttackCollider*>(other);
        if (!receivedAttackIds_.insert(attack->GetAttackId()).second) return;

        if (pHp_)
            pHp_->Damage(25); // TODO : 仮のダメージ値

        // SEを流す
        ozSound::SoundEngine::GetInstance()->PostEvent("play_se_enemy_take_hit");
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
