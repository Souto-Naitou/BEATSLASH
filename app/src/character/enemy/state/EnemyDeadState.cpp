#include "EnemyDeadState.h"
#include <character/enemy/Enemy.h>
#include <FrameTimer.h>

namespace
{
	float EaseOutBounce(float x)
	{
		const float n1 = 7.5625f;
		const float d1 = 2.75f;

		if (x < 1.0f / d1)
		{
			return n1 * x * x;
		}
		else if (x < 2.0f / d1)
		{
			x -= 1.5f / d1;
			return n1 * x * x + 0.75f;
		}
		else if (x < 2.5f / d1)
		{
			x -= 2.25f / d1;
			return n1 * x * x + 0.9375f;
		}
		else
		{
			x -= 2.625f / d1;
			return n1 * x * x + 0.984375f;
		}
	}
}

EnemyDeadState::EnemyDeadState(Tako::EmitterManager* emitterManager)
	: pEmitterManager_(emitterManager)
{
}

void EnemyDeadState::Enter(Enemy* enemy)
{
	timer_ = 0.0f;
	if (enemy)
	{
		startScale_ = enemy->GetScale();
		startPosition_ = enemy->GetPosition();
		// 死亡演出開始時に当たり判定を無効化する
		enemy->DisableCollider();
	}
}

void EnemyDeadState::Update(Enemy* enemy)
{
	if (!enemy)
	{
		return;
	}

	timer_ += Tako::FrameTimer::GetInstance()->GetDeltaTime();
	float t = timer_ / kDeadDuration_;

	// 位置が変わらないようにする
	enemy->SetPosition(startPosition_);

	if (t >= 1.0f)
	{
		t = 1.0f;
		enemy->SetScale({ 0.0f, 0.0f, 0.0f });
		// 演出終了を通知
		enemy->SetDeadFinished(true);
	}
	else
	{
		// バウンスしながら縮小させる
		float easedT = 1.0f - EaseOutBounce(t);
		enemy->SetScale(startScale_ * easedT);
	}
}
