#pragma once
#include <characters/base/Character.h>
#include <characters/enemies/state/base/EnemyStateMachine.h>

class Enemy : public Character
{
public:
	Enemy() = default;
	~Enemy() override = default;
	void Initialize(CharacterColliderID colliderID) override;
	void Update() override;

private:
	// 敵のステートマシン
	EnemyStateMachine stateMachine_;
};

