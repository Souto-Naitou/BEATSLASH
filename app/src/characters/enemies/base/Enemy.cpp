#include "Enemy.h"

void Enemy::Initialize(CharacterColliderID colliderID)
{
	Character::Initialize(colliderID);
}

void Enemy::Update()
{
	// ステートマシンの更新
	stateMachine_->Update(this);
}

