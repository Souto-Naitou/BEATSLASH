#pragma once
#include <character/enemy/state/EnemyState.h>
#include <character/enemy/state/EnemyStateType.h>
#include <character/ICharacter.h>
#include <optional>
#include <Vector3.h>
#include <debug/GameParameter.h>

class EnemyChaseState : public EnemyState
{
public:
	EnemyChaseState(const ICharacter* target);
	void Enter(Enemy* enemy) override;
	void Update(Enemy* enemy) override;
	void DrawImGui(Enemy* enemy) override;
	std::optional<EnemyStateType> CheckTransition(Enemy* enemy) override;

private:
	// 追跡を諦めて待機に戻る距離
	static constexpr float kChaseEndDistance = 25.0f;
	// 攻撃を開始する距離
	static constexpr float kAttackStartDistance = 1.0f;

	// 追跡ターゲット
	const ICharacter* pTarget_ = nullptr;

	// デバッグ用のパラメータ
	EnableDebug("EnemyChaseState");
	// 追跡速度
	GameParameter(float, chaseSpeed_, 5.0f);
};

