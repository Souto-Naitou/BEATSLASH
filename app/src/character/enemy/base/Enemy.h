#pragma once
#include <character/base/ICharacter.h>
#include <character/enemy/state/base/EnemyStateMachine.h>

class Enemy : public ICharacter
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

