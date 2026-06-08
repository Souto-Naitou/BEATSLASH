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
	// 追跡処理
	void Chase(Enemy* enemy, float deltaTime);

	// 常にターゲットの方を向く処理
	void FaceTarget(Enemy* enemy);

	// 攻撃のクールタイム処理
	void UpdateAttackCooldown(float deltaTime);

private:
	// 追跡を諦めて待機に戻る距離
	static constexpr float kChaseEndDistance = 25.0f;
	// 攻撃を開始する距離
	static constexpr float kAttackStartDistance = 7.0f;
	// 攻撃のクールタイム
	static constexpr float kAttackCooldown = 1.0f;
	// 攻撃のクールタイム（拍数）
	static constexpr float kAttackCooldownBeats = 4.0f;
	// 攻撃クールタイムのタイマー
	float attackCooldownTimer_ = 0.0f;
	// 追跡ターゲット
	const ICharacter* pTarget_ = nullptr;
	// 前フレームの拍インデックス
	int lastBeatIndex_ = -1;

	// デバッグ用のパラメータ
	EnableDebug("EnemyChaseState");
	// 追跡速度
	GameParameter(float, chaseSpeed_, 7.0f);
};

