#include "EnemySpawnState.h"
#include <character/enemy/Enemy.h>
#include <FrameTimer.h>
#include <cmath>
#include <EmitterManager.h>
#include <ozSound/audio/SoundEngine.h>

uint32_t EnemySpawnState::effectCount_ = 0;

EnemySpawnState::EnemySpawnState(Tako::EmitterManager* emitterManager)
	: pEmitterManager_(emitterManager)
{
	// スポーンの際に使うエフェクトのロード
	pEmitterManager_->LoadPreset("enemy_spawn");
	pEmitterManager_->SetEmitterActive("enemy_spawn", false);
}

void EnemySpawnState::Enter(Enemy* enemy)
{
	timer_ = 0.0f;
	targetScale_ = enemy->GetScale();

	// スポーン時の配置方向を目標とする
	targetAngle_ = enemy->GetRotation().y;

	// 初期状態の設定（スケール0、目標角度から回転した状態）
	enemy->SetScale({ 0.0f, 0.0f, 0.0f });
	enemy->SetRotation({ 0.0f, targetAngle_ - kSpinAngle, 0.0f });

	// スポーンエフェクトの再生
	if (pEmitterManager_)
	{
		std::string effectName = "enemy_spawn_" + std::to_string(effectCount_++);
		pEmitterManager_->CreateTemporaryEmitterFrom("enemy_spawn", effectName, 1.0f);
		pEmitterManager_->SetEmitterPosition(effectName, enemy->GetPosition());
	}

	// 音再生フラグのリセット
	hasPlayedSound_ = false;
}

namespace
{
	/**
	 * @brief EaseInOutCirc の計算をする。
	 * @param x 0から1の進捗
	 */
	float EaseInOutCirc(float x)
	{
		return x < 0.5f
			? (1.0f - std::sqrt(1.0f - std::pow(2.0f * x, 2.0f))) / 2.0f
			: (std::sqrt(1.0f - std::pow(-2.0f * x + 2.0f, 2.0f)) + 1.0f) / 2.0f;
	}
}

void EnemySpawnState::Update(Enemy* enemy)
{
	float deltaTime = Tako::FrameTimer::GetInstance()->GetDeltaTime();
	timer_ += deltaTime;
	if (timer_ > kDuration)
	{
		timer_ = kDuration;
	}

	float p = timer_ / kDuration;
	// イージング（EaseInOutCirc）
	float e = EaseInOutCirc(p);

	// スケールの設定
	enemy->SetScale(targetScale_ * e);

	// 回転の設定
	float angle = targetAngle_ - kSpinAngle * (1.0f - e);
	enemy->SetRotation({ 0.0f, angle, 0.0f });

	// 途中まで経過したらスポーンサウンドを流す
	if (p >= 0.3f && !hasPlayedSound_)
	{
		hasPlayedSound_ = true;
		ozSound::SoundEngine::GetInstance()->PostEvent("play_se_enemy_spawn");
	}
}

std::optional<EnemyStateType> EnemySpawnState::CheckTransition(Enemy* enemy)
{
	if (timer_ >= kDuration)
	{
		// 演出終了後は待機状態に遷移する
		return EnemyStateType::Idle;
	}
	return std::nullopt;
}
