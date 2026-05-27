#pragma once
#include "EnemyState.h"
#include <memory>
#include "character/enemy/collider/EnemyAttackCollider.h"

class EnemyAttackState : public EnemyState
{
public:
	void Enter(Enemy* enemy) override;
	void Update(Enemy* enemy) override;
	void DrawImGui(Enemy* enemy) override;

private:
	// 攻撃のコライダー
	std::unique_ptr<EnemyAttackCollider> pAttackCollider_;
};

