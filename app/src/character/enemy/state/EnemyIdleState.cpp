#include "EnemyIdleState.h"
#include <FrameTimer.h>
#include <character/enemy/Enemy.h>

void EnemyIdleState::Enter(Enemy* enemy)
{
	timer_ = 0.0f; // タイマーをリセット
}

void EnemyIdleState::Update(Enemy* enemy)
{
	// タイマーの加算
	timer_ += Tako::FrameTimer::GetInstance()->GetDeltaTime();

	// モデルを少し大きくさせたり小さくさせてリズムに乗っているような動きをさせる
	float scale = baseScale_ + scaleAmplitude_ * std::sin(timer_ * scaleSpeed_);
	enemy->GetModel()->SetScale({ scale, scale, scale });
}
