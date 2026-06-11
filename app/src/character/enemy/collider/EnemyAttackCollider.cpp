#include "EnemyAttackCollider.h"
#include <type/ColliderTypeID.h>
#include <EmitterManager.h>
#include <Collider.h>

EnemyAttackCollider::EnemyAttackCollider(Tako::EmitterManager* emitterManager)
	: pEmitterManager_(emitterManager)
{
}

void EnemyAttackCollider::OnCollisionEnter(Tako::Collider* other)
{
	ColliderTypeID otherID = static_cast<ColliderTypeID>(other->GetTypeID());
	if (otherID == ColliderTypeID::Player)
	{
	}
}

void EnemyAttackCollider::OnCollisionStay(Tako::Collider* other)
{

}

void EnemyAttackCollider::OnCollisionExit(Tako::Collider* other)
{

}