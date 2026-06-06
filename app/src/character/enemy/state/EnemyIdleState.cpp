#include "EnemyIdleState.h"
#include <character/ICharacter.h>
#include <FrameTimer.h>
#include <character/enemy/Enemy.h>

std::optional<EnemyStateType> EnemyIdleState::CheckTransition(Enemy* enemy)
{
	if (!pTarget_)
	{
		return std::nullopt; // ターゲットが存在しない場合は遷移なし
	}

	// ターゲットとの距離を計算
	Tako::Vector3 toTarget = pTarget_->GetPosition() - enemy->GetPosition();
	float distanceSq = toTarget.LengthSquared();

	// 追跡距離以内にターゲットがいる場合は、Chase状態に遷移
	if (distanceSq < kChaseStartDistance * kChaseStartDistance)
	{
		return EnemyStateType::Chase;
	}

	// それ以外は遷移なし
	return std::nullopt;
}

void EnemyIdleState::Enter(Enemy* enemy)
{
	timer_ = 0.0f; // タイマーをリセット

	// わかりやすいように、待機状態に入ったときに色を変える
	enemy->GetModel()->SetMaterialColor({ 0,256,0,256 });
}

void EnemyIdleState::Update(Enemy* enemy)
{
	// タイマーの加算
	timer_ += Tako::FrameTimer::GetInstance()->GetDeltaTime();

	// モデルを少し大きくさせたり小さくさせてリズムに乗っているような動きをさせる
	float scale = baseScale_ + scaleAmplitude_ * std::sin(timer_ * scaleSpeed_);
	enemy->SetScale({ scale, scale, scale });
}

void EnemyIdleState::DrawImGui(Enemy* enemy)
{
	// 処理なし
}