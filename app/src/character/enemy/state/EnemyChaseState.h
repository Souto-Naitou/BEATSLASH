#pragma once
#include "EnemyState.h"
#include <Vector3.h>

class ICharacter;

class EnemyChaseState : public EnemyState
{
public:
	/**
	 * @brief コンストラクタ
	 * @param target 追跡ターゲット（所有しない）
	 */
	EnemyChaseState(const ICharacter* target);

	void Enter(Enemy* enemy) override;
	void Update(Enemy* enemy) override;
	void DrawImGui(Enemy* enemy) override;

private:
	// 追跡ターゲット
	const ICharacter* pTarget_ = nullptr;

	// 追跡速度
	float chaseSpeed_ = 5.0f;
};

