#include "EnemyChaseState.h"
#include <character/ICharacter.h>
#include <character/enemy/Enemy.h>
#include <FrameTimer.h>
#include <imgui.h>

EnemyChaseState::EnemyChaseState(const ICharacter* target)
	: pTarget_(target)
{
}

void EnemyChaseState::Enter(Enemy* enemy)
{
	// わかりやすいように、追跡状態に入ったときに色を変える
	enemy->GetModel()->SetMaterialColor({ 256,0,0,256 });
}

void EnemyChaseState::Update(Enemy* enemy)
{
	// ターゲットが存在しない場合は処理を終了
	if (!pTarget_)
	{
		return;
	}

	// デルタタイムの取得
	float deltaTime = Tako::FrameTimer::GetInstance()->GetDeltaTime();

	// 追跡処理
	Chase(enemy, deltaTime);

	// 常にターゲットの方を向く処理
	FaceTarget(enemy);

	// 攻撃クールタイムの更新
	UpdateAttackCooldown(deltaTime);
}

void EnemyChaseState::DrawImGui(Enemy* enemy)
{
	if (pTarget_)
	{
		// ターゲットの座標を表示
		Tako::Vector3 pos = pTarget_->GetPosition();
		ImGui::Text("Target Position: %.2f, %.2f, %.2f", pos.x, pos.y, pos.z);

		// ターゲットまでの距離を表示
		Tako::Vector3 toTarget = pTarget_->GetPosition() - enemy->GetPosition();
		float distanceSq = toTarget.LengthSquared();
		ImGui::Text("DistanceSquared : %.2f, Distance : %.2f", distanceSq, std::sqrt(distanceSq));
	}
}

std::optional<EnemyStateType> EnemyChaseState::CheckTransition(Enemy* enemy)
{
	if (!pTarget_)
	{
		return EnemyStateType::Idle; // ターゲットを失ったら待機に戻る
	}

	// 自身とターゲットの座標から距離を計算
	Tako::Vector3 direction = pTarget_->GetPosition() - enemy->GetPosition();
	float distanceSq = direction.LengthSquared();

	// 攻撃開始距離かつ攻撃のクールタイムが終了している場合は攻撃状態へ遷移
	if (distanceSq <= kAttackStartDistance * kAttackStartDistance && attackCooldownTimer_ <= 0.0f)
	{
		// 攻撃クールタイムのリセット
		attackCooldownTimer_ = kAttackCooldown;
		
		return EnemyStateType::Attack;
	}

	// 追跡限界距離より離れたら待機状態へ遷移
	if (distanceSq >= kChaseEndDistance * kChaseEndDistance)
	{
		return EnemyStateType::Idle;
	}

	return std::nullopt;
}

void EnemyChaseState::Chase(Enemy* enemy, float deltaTime)
{
	// 自身とターゲットの座標を取得
	Tako::Vector3 currentPos = enemy->GetPosition();
	Tako::Vector3 targetPos = pTarget_->GetPosition();

	// ターゲットのY座標はスケールに応じて変わるので、スケールの分だけ下にずらす
	targetPos.y -= pTarget_->GetScale().y * 0.5f;

	// ターゲットへの方向ベクトルを計算
	Tako::Vector3 direction = targetPos - currentPos;

	// ターゲットまでの平方距離を計算
	float distanceSq = direction.LengthSquared();

	// １フレーム当たりの移動量を計算
	float moveAmount = chaseSpeed_ * deltaTime;

	// 到達判定
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

void EnemyChaseState::FaceTarget(Enemy* enemy)
{
	// 常にプレイヤーの方を向くようにする
	if (pTarget_)
	{
		// 自身の座標を取得
		Tako::Vector3 currentPos = enemy->GetPosition();

		// ターゲットへのベクトルを計算
		Tako::Vector3 toTarget = pTarget_->GetPosition() - currentPos;

		// Y軸を回転させる
		float angle = std::atan2(toTarget.x, toTarget.z);
		
		// 回転を設定
		enemy->SetRotation({ 0.0f, angle, 0.0f });
	}
}

void EnemyChaseState::UpdateAttackCooldown(float deltaTime)
{
	// クールタイムが0未満の場合は処理を行わない。
	if (attackCooldownTimer_ < 0.0f)
	{
		return;
	}

	// 攻撃クールタイムの更新
	attackCooldownTimer_ -= deltaTime;
	if (attackCooldownTimer_ < 0.0f)
	{
		attackCooldownTimer_ = 0.0f; // クールタイムが0未満にならないようにする
	}
}