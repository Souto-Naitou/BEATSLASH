#include "PlayerAttackCollider.h"



void PlayerAttackCollider::Update()
{
    pColliderTimer_->UpdateTime();
    pColliderTimer_->UpdateColliderState(this);
}

void PlayerAttackCollider::OnCollisionEnter(Collider* other)
{
}

void PlayerAttackCollider::OnCollisionStay(Collider* other)
{
}

void PlayerAttackCollider::OnCollisionExit(Collider* other)
{
}
