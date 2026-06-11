#pragma once

#include <character/enemy/state/EnemyState.h>
#include <character/enemy/state/EnemyStateType.h>
#include <character/ICharacter.h>
#include <optional>
#include <Vector3.h>

namespace Tako
{
	class EmitterManager;
}

/**
 * @brief 敵の出現（スポーン）演出用ステートクラス
 */
class EnemySpawnState : public EnemyState
{
public:
	/**
	 * @brief コンストラクタ
	 */
	EnemySpawnState(Tako::EmitterManager* emitterManager = nullptr);

	std::optional<EnemyStateType> CheckTransition(Enemy* enemy) override;
	void Enter(Enemy* enemy) override;
	void Update(Enemy* enemy) override;

private:
	// エミッターマネージャーのポインタ
	Tako::EmitterManager* pEmitterManager_ = nullptr;
	
	// 演出時間（秒）
	static constexpr float kDuration = 1.2f;
	// 回転量（ラジアン）: 4回転（8*pi）
	static constexpr float kSpinAngle = 8.0f * 3.1415926535f;
	// エフェクトカウント
	static uint32_t effectCount_;
	// 音再生フラグ
	bool hasPlayedSound_ = false;
	// 経過時間タイマー
	float timer_ = 0.0f;
	// 目標スケール
	Tako::Vector3 targetScale_ = { 1.0f, 1.0f, 1.0f };
	// 目標角度（Y軸）
	float targetAngle_ = 0.0f;
	
};
