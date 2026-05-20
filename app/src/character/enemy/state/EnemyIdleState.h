#pragma once

#include <character/enemy/state/EnemyState.h>

class EnemyIdleState : public EnemyState
{
public:
	~EnemyIdleState() override = default;
	
	void Enter(Enemy* enemy) override;
	void Update(Enemy* enemy) override;

private:
	// 経過時間
	float timer_ = 0.0f;
	// 拡縮速度
	float scaleSpeed_ = 14.0f;
	// 拡縮幅
	float scaleAmplitude_ = 0.1f;
	// 基本のスケール
	float baseScale_ = 1.0f;

};

