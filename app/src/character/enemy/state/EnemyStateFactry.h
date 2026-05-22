#pragma once

#include <character/enemy/state/EnemyStateType.h>
#include <character/enemy/state/EnemyState.h>
#include <memory>
#include "EnemyIdleState.h"
#include "EnemyChaseState.h"

class ICharacter;

class EnemyStateFactory
{
public:
	static std::unique_ptr<EnemyState> Create(EnemyStateType type, const ICharacter* target)
	{
		switch (type)
		{
		case EnemyStateType::Idle:
			return std::make_unique<EnemyIdleState>();
		case EnemyStateType::Chase:
			return std::make_unique<EnemyChaseState>(target);
		case EnemyStateType::Attack:
			//return std::make_unique<AttackState>();
		case EnemyStateType::Dead:
			//return std::make_unique<DeadState>();
		default:
			return nullptr;
		}
	}
};