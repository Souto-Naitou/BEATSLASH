#pragma once

class Enemy;

class EnemyState
{
public:
	virtual ~EnemyState() = default;

	virtual void Enter(Enemy* enemy) {}		// 状態に入るときの処理
	virtual void Update(Enemy* enemy) {}	// 状態の更新処理
	virtual void Exit(Enemy* enemy) {}		// 状態から出るときの処理
};