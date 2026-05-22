#include "EnemyChaseState.h"
#include <character/enemy/Enemy.h>
#include <FrameTimer.h>
#include <imgui.h>

EnemyChaseState::EnemyChaseState(const ICharacter* target)
	: pTarget_(target)
{
}

void EnemyChaseState::Enter(Enemy* enemy)
{
	// 処理なし
}

void EnemyChaseState::Update(Enemy* enemy)
{
	// デルタタイムの取得
	float deltaTime = Tako::FrameTimer::GetInstance()->GetDeltaTime();

	// 自身とターゲットの座標を取得
	Tako::Vector3 currentPos = enemy->GetPosition();	
	if (!pTarget_)
	{
		return; // ターゲットが存在しない場合は処理を終了
	}
	Tako::Vector3 targetPos = pTarget_->GetPosition();

	// ターゲットのY座標はスケールに応じて変わるので、スケールの分だけ下にずらす
	targetPos.y -= pTarget_->GetScale().y * 0.5f;

	// ターゲットへの方向ベクトルを計算
	Tako::Vector3 direction = targetPos - currentPos;

	// ターゲットまでの平方距離を計算
	float distanceSq = direction.LengthSquared();

	// １フレーム当たりの移動量を計算
	float moveAmount = chaseSpeed_ * deltaTime;

	// 到達判定。ターゲットまでの距離が移動量より小さい場合は、ターゲットに到達したとみなす。
	if (distanceSq <= moveAmount * moveAmount)
	{
		enemy->SetPosition(targetPos); // ターゲットの位置に直接設定
		return;
	}

	// 方向ベクトルの正規化
	direction = direction.Normalize();

	// 自身の座標を更新
	currentPos += direction * moveAmount;

	// 更新した座標を自身に設定
	enemy->SetPosition(currentPos);
}

void EnemyChaseState::DrawImGui(Enemy* enemy)
{
	if (pTarget_)
	{
		Tako::Vector3 pos = pTarget_->GetPosition();
		ImGui::Text("Target Position: %.2f, %.2f, %.2f", pos.x, pos.y, pos.z);
	}
	else
	{
		ImGui::Text("Target: None");
	}
}

