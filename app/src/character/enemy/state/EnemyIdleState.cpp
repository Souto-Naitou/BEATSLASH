#include "EnemyIdleState.h"
#include <character/ICharacter.h>
#include <FrameTimer.h>
#include <character/enemy/Enemy.h>
#include <manager/BeatManager.h>

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
}

void EnemyIdleState::Update(Enemy* enemy)
{
	// 拡縮アニメーションはEnemy::Updateでグローバルに行われるため、ステート内での個別処理は不要
}

void EnemyIdleState::DrawImGui(Enemy* enemy)
{
	// 処理なし
}