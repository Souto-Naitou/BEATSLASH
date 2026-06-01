#pragma once
#include <string>

enum class EnemyStateType
{
	Idle,		// 待機状態
	Chase,		// 追跡状態
	Attack,		// 攻撃状態
	Dead		// 死亡状態
};

inline const std::string GetStateName(EnemyStateType type)
{
	switch (type)
	{
	case EnemyStateType::Idle:
		return "Idle";
	case EnemyStateType::Chase:
		return "Chase";
	case EnemyStateType::Attack:
		return "Attack";
	default:
		return "Unknown";
	}
}