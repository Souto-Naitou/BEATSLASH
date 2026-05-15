#pragma once

#include <characters/enemies/state/base/EnemyStateType.h>

class EnemyStateFactory
{
public:
	static std::unique_ptr<EnemyState> Create(EnemyStateType type)
	{
		switch (type)
		{
		case EnemyStateType::Idle:
			//return std::make_unique<IdleState>();
		case EnemyStateType::Chase:
			//return std::make_unique<ChaseState>();
		case EnemyStateType::Attack:
			//return std::make_unique<AttackState>();
		case EnemyStateType::Dead:
			//return std::make_unique<DeadState>();
		default:
			return nullptr;
		}
	}
};