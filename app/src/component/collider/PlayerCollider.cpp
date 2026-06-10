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
    else if(otherID == ColliderTypeID::Enemy || otherID == ColliderTypeID::EnemyAttack)
    {
        /// 敵との衝突、または敵の攻撃と衝突した場合の処理
        if (!parryJudgement_.Judge())
        {
            /// パリィ失敗時の処理
            comboBuffSystem_.OnDamaged();
            hpComponent_.Damage(10); // 仮のダメージ量
        }
        else
        {
            /// パリィ成功時の処理 (外部)
            parrySuccessCallback_();
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
