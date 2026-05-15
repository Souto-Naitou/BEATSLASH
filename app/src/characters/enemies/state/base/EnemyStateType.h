#pragma once

enum class EnemyStateType
{
	Idle,		// 待機状態
	Chase,		// 追跡状態
	Attack,		// 攻撃状態
	Dead		// 死亡状態
};