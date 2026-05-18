#include "EnemyChaseState.h"
#include <character/enemy/Enemy.h>
#include <FrameTimer.h>
#include <imgui.h>

void EnemyChaseState::Enter(Enemy* enemy)
{
	if(!pInput_)
	{
		pInput_ = Tako::Input::GetInstance();
	}
}

void EnemyChaseState::Update(Enemy* enemy)
{
	// ターゲットの座標を更新
	TargetUpdate();

	// デルタタイムの取得
	float deltaTime = Tako::FrameTimer::GetInstance()->GetDeltaTime();

	// 自身とターゲットの座標を取得
	// NOTE: ターゲットは一旦ローカル変数で座標を指定しておく。
	Tako::Vector3 currentPos = enemy->GetPosition();	
	//Tako::Vector3 targetPos = player->GetPosition();

	// ターゲットへの方向ベクトルを計算
	Tako::Vector3 direction = targetPos_ - currentPos;

	// ターゲットまでの平方距離を計算
	float distanceSq = direction.LengthSquared();

	// １フレーム当たりの移動量を計算
	float moveAmount = chaseSpeed_ * deltaTime;

	// 到達判定。ターゲットまでの距離が移動量より小さい場合は、ターゲットに到達したとみなす。
	if(distanceSq <= moveAmount * moveAmount) {
		enemy->SetPosition(targetPos_); // ターゲットの位置に直接設定
		return; // 更新処理を終了
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
	ImGui::SliderFloat3("Target Position", &targetPos_.x, -5.0f, 5.0f);
}

void EnemyChaseState::TargetUpdate()
{
	if (!pInput_)
	{
		return;
	}

	// 入力を受けてターゲットの位置を更新する（仮の実装）
	if (pInput_->PushKey(DIK_W))
	{
		targetPos_.z += 1.0f; // 前に移動
	}
	if(pInput_->PushKey(DIK_S))
	{
		targetPos_.z -= 1.0f; // 後ろに移動
	}
	if(pInput_->PushKey(DIK_A))
	{
		targetPos_.x -= 1.0f; // 左に移動
	}
	if( pInput_->PushKey(DIK_D))
	{
		targetPos_.x += 1.0f; // 右に移動
	}

	// ターゲットの移動幅を5.0fに制限
	if (targetPos_.x > 5.0f) {
		targetPos_.x = 5.0f;
	}
	else if (targetPos_.x < -5.0f) {
		targetPos_.x = -5.0f;
	}

	if (targetPos_.z > 5.0f) {
		targetPos_.z = 5.0f;
	}
	else if (targetPos_.z < -5.0f) {
		targetPos_.z = -5.0f;
	}
}
