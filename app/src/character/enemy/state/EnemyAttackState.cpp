#include "EnemyAttackState.h"
#include "character/enemy/Enemy.h"
#include <type/ColliderTypeID.h>
#include <CollisionManager.h>
#include <FrameTimer.h>

EnemyAttackState::EnemyAttackState(const ICharacter* target)
	: pTarget_(target)
{
}

void EnemyAttackState::Enter(Enemy* enemy)
{
	// タイマーのリセット
	timer_ = 0.0f;

	// コライダートランスフォームの初期化
	colliderTransform_ = enemy->GetTransform();
	float yaw = colliderTransform_.rotate.y;
	Tako::Vector3 forward = { std::sin(yaw), 0.0f, std::cos(yaw) };
	colliderTransform_.translate += forward * kColliderOffset;

	// 攻撃コライダーの生成と初期化
	pAttackCollider_ = std::make_unique<EnemyAttackCollider>();
	pAttackCollider_->SetSize({ 2.0f, 2.0f, 2.0f });
	pAttackCollider_->SetTransform(&colliderTransform_);
	pAttackCollider_->SetTypeID(static_cast<uint32_t>(ColliderTypeID::EnemyAttack));
	pAttackCollider_->SetOwner(this);

	// コライダーをマネージャーに登録
	auto collisionManager = Tako::CollisionManager::GetInstance();
	collisionManager->AddCollider(pAttackCollider_.get());
	collisionManager->SetCollisionMask(static_cast<uint32_t>(ColliderTypeID::EnemyAttack), static_cast<uint32_t>(ColliderTypeID::Player), true);
}

void EnemyAttackState::Update(Enemy* enemy)
{
	// 時間の加算
	timer_ += Tako::FrameTimer::GetInstance()->GetDeltaTime();

}

void EnemyAttackState::Exit(Enemy* enemy)
{
	// コライダーをマネージャーから削除
	auto collisionManager = Tako::CollisionManager::GetInstance();
	collisionManager->RemoveCollider(pAttackCollider_.get());
	pAttackCollider_ = nullptr;
}

void EnemyAttackState::DrawImGui(Enemy* enemy)
{

}

std::optional<EnemyStateType> EnemyAttackState::CheckTransition(Enemy* enemy)
{
	// 攻撃時間が経過したら追従状態に戻る
	if (timer_ >= kAttackDuration_)
	{
		return EnemyStateType::Chase;
	}
	return std::nullopt;
}