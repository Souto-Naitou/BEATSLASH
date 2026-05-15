#include "Enemy.h"

void Enemy::Initialize(CharacterColliderID colliderID)
{
	ICharacter::Initialize(colliderID);
}

void Enemy::Update()
{
	// ステートマシンの更新
	stateMachine_.Update();
}

