#include "EnemyAttackState.h"
#include "character/enemy/Enemy.h"
#include <type/ColliderTypeID.h>
#include <CollisionManager.h>
#include <FrameTimer.h>

void EnemyAttackState::Enter(Enemy* enemy)
{
	timer_ = 0.0f; // タイマーをリセット

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

void EnemyAttackState::Update(Enemy* enemy)
{
	// 経過時間の加算
	timer_ += Tako::FrameTimer::GetInstance()->GetDeltaTime();
}

void EnemyAttackState::DrawImGui(Enemy* enemy)
{

}

std::optional<EnemyStateType> EnemyAttackState::CheckTransition(Enemy* enemy)
{
	// 攻撃時間が経過したら追従状態に戻る
	if (timer_ >= kAttackDuration)
	{
		return EnemyStateType::Chase;
	}
	return std::nullopt;
}