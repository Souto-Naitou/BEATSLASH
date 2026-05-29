#pragma once
#include <character/enemy/state/EnemyState.h>
#include <character/enemy/state/EnemyStateType.h>
#include <optional>
#include <memory>
#include "character/enemy/collider/EnemyAttackCollider.h"

class EnemyAttackState : public EnemyState
{
public:
	void Enter(Enemy* enemy) override;
	void Update(Enemy* enemy) override;
	void DrawImGui(Enemy* enemy) override;
	std::optional<EnemyStateType> CheckTransition(Enemy* enemy) override;

private:
	// 攻撃の持続時間
	static constexpr float kAttackDuration = 1.0f;

	// 攻撃のコライダー
	std::unique_ptr<EnemyAttackCollider> pAttackCollider_;
	// 経過時間
	float timer_ = 0.0f;
};

