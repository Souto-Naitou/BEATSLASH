#pragma once

#include <character/enemy/state/base/EnemyState.h>

class EnemyIdleState : public EnemyState
{
public:
	~EnemyIdleState() override = default;
	
	void Enter(Enemy* enemy) override;
	void Update(Enemy* enemy) override;

private:
	float timer_ = 0.0f;
};

