#pragma once

#include <character/enemy/state/EnemyState.h>
#include <character/enemy/state/EnemyStateType.h>
#include <character/ICharacter.h>
#include <optional>
#include <Vector3.h>

/**
 * @brief 敵の出現（スポーン）演出用ステートクラス
 */
class EnemySpawnState : public EnemyState
{
public:
	/**
	 * @brief コンストラクタ
	 */
	EnemySpawnState();

	std::optional<EnemyStateType> CheckTransition(Enemy* enemy) override;
	void Enter(Enemy* enemy) override;
	void Update(Enemy* enemy) override;

private:
	// 演出時間（秒）
	static constexpr float kDuration = 1.2f;
	// 回転量（ラジアン）: 4回転（8*pi）
	static constexpr float kSpinAngle = 8.0f * 3.1415926535f;

	// 経過時間タイマー
	float timer_ = 0.0f;
	// 目標スケール
	Tako::Vector3 targetScale_ = { 1.0f, 1.0f, 1.0f };
	// 目標角度（Y軸）
	float targetAngle_ = 0.0f;
};
