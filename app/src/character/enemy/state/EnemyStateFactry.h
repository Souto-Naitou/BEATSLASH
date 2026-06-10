#pragma once

#include <character/enemy/state/EnemyStateType.h>
#include <character/enemy/state/EnemyState.h>
#include <memory>
#include "EnemySpawnState.h"
#include "EnemyIdleState.h"
#include "EnemyChaseState.h"
#include "EnemyAttackState.h"
#include "EnemyDeadState.h"

class ICharacter;
namespace Tako
{
	class EmitterManager;
}

class EnemyStateFactory
{
public:
	static std::unique_ptr<EnemyState> Create(EnemyStateType type, const ICharacter* target, Tako::EmitterManager* emitterManager = nullptr)
	{
		switch (type)
		{
		case EnemyStateType::Spawn:
			return std::make_unique<EnemySpawnState>();
		case EnemyStateType::Idle:
			return std::make_unique<EnemyIdleState>(target);
		case EnemyStateType::Chase:
			return std::make_unique<EnemyChaseState>(target);
		case EnemyStateType::Attack:
			return std::make_unique<EnemyAttackState>(target, emitterManager);
		case EnemyStateType::Dead:
			return std::make_unique<EnemyDeadState>(emitterManager);
		default:
			return nullptr;
		}
	}
};