#include "EnemyAttackState.h"
#include "character/enemy/Enemy.h"
#include <type/ColliderTypeID.h>
#include <CollisionManager.h>

void EnemyAttackState::Enter(Enemy* enemy)
{
	// 攻撃コライダーの生成と初期化
	pAttackCollider_ = std::make_unique<EnemyAttackCollider>();
	pAttackCollider_->SetSize({ 1.0f, 1.0f, 1.0f });
	pAttackCollider_->SetTransform(&enemy->GetTransform());
	pAttackCollider_->SetTypeID(static_cast<uint32_t>(ColliderTypeID::EnemyAttack));
	pAttackCollider_->SetOwner(this);

	// コライダーをマネージャーに登録
	auto collisionManager = Tako::CollisionManager::GetInstance();
	collisionManager->AddCollider(pAttackCollider_.get());
	collisionManager->SetCollisionMask(static_cast<uint32_t>(ColliderTypeID::EnemyAttack), static_cast<uint32_t>(ColliderTypeID::Player), true);
}

void EnemyAttackState::Update(Enemy * enemy)
{

}

void EnemyAttackState::DrawImGui(Enemy * enemy)
{

}
