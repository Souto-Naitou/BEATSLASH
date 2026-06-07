#pragma once
#include <character/enemy/state/EnemyState.h>
#include <character/enemy/state/EnemyStateType.h>
#include <optional>
#include <memory>
#include "character/enemy/collider/EnemyAttackCollider.h"
#include <character/ICharacter.h>
#include <Object3d.h>

class EnemyAttackState : public EnemyState
{
public:
	EnemyAttackState(const ICharacter* target = nullptr);
	void Enter(Enemy* enemy) override;
	void Update(Enemy* enemy) override;
	void Exit(Enemy* enemy) override;
	void Draw(Enemy* enemy) override;
	void DrawImGui(Enemy* enemy) override;
	std::optional<EnemyStateType> CheckTransition(Enemy* enemy) override;

private:
	// ターゲットのポインタ
	const ICharacter* pTarget_ = nullptr;

	// 攻撃のコライダー
	std::unique_ptr<EnemyAttackCollider> pAttackCollider_;
	// 攻撃エフェクトモデル
	std::unique_ptr<Tako::Object3d> pAttackModel_;
	// コライダーのトランスフォーム
	Tako::Transform colliderTransform_;
	// 攻撃の持続時間
	static constexpr float kAttackDuration = 1.0f;
	// 攻撃持続時間
	static constexpr float kAttackDuration_ = 1.0f;
	// 攻撃コライダーの出現オフセット
	static constexpr float kColliderOffset = 5.0f;
	// 攻撃時間のタイマー
	float timer_ = 0.0f;
	
};

