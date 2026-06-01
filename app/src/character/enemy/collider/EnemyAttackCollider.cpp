#include "EnemyAttackCollider.h"
#include <type/ColliderTypeID.h>

void EnemyAttackCollider::OnCollisionEnter(Tako::Collider* other)
{
	ColliderTypeID otherID = static_cast<ColliderTypeID>(other->GetTypeID());
	if (otherID == ColliderTypeID::Player)
	{
		// プレイヤーにダメージを与える処理をここに追加
	}
}

void EnemyAttackCollider::OnCollisionStay(Tako::Collider* other)
{

}

void EnemyAttackCollider::OnCollisionExit(Tako::Collider* other)
{

}