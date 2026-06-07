#include "EnemyAttackState.h"
#include "character/enemy/Enemy.h"
#include <type/ColliderTypeID.h>
#include <CollisionManager.h>
#include <FrameTimer.h>
#include <cmath>

EnemyAttackState::EnemyAttackState(const ICharacter* target)
	: pTarget_(target)
{
}

void EnemyAttackState::Enter(Enemy* enemy)
{
	// タイマーのリセット
	timer_ = 0.0f;

	// 敵の現在の回転（基準方向）を取得
	float baseYaw = enemy->GetTransform().rotate.y;
	
	// 開始角度は右に60度（DirectXや数学座標系において、右から左へ薙ぎ払う）
	float startAngle = baseYaw + 3.14159265f / 3.0f;

	// コライダートランスフォームの初期化
	colliderTransform_ = enemy->GetTransform();
	Tako::Vector3 forward = { std::sin(startAngle), 0.0f, std::cos(startAngle) };
	colliderTransform_.translate += forward * kColliderOffset;
	colliderTransform_.translate.y += 0.5f; // 少し浮かす
	colliderTransform_.rotate = { 0.0f, startAngle + 3.14159265f / 2.0f, 0.0f }; // 接線方向に向ける
	colliderTransform_.scale = { 1.3f, 0.1f, 0.3f }; // コライダーを薄長くする

	// 攻撃モデルの生成と初期化
	pAttackModel_ = std::make_unique<Tako::Object3d>();
	pAttackModel_->Initialize();
	pAttackModel_->SetModel("white_cube.gltf");
	pAttackModel_->SetMaterialColor({ 256, 128, 0, 256 }); // オレンジ色
	pAttackModel_->SetEnableLighting(true);
	pAttackModel_->SetTransform(colliderTransform_);
	pAttackModel_->Update();

	// 攻撃コライダーの生成と初期化
	pAttackCollider_ = std::make_unique<EnemyAttackCollider>();
	pAttackCollider_->SetSize(pAttackModel_->GetScale() * 3.0f); // コライダーは少し大きめにする
	pAttackCollider_->SetTransform(&colliderTransform_);
	pAttackCollider_->SetTypeID(static_cast<uint32_t>(ColliderTypeID::EnemyAttack));
	pAttackCollider_->SetOwner(this);

	// コライダーをマネージャーに登録
	auto collisionManager = Tako::CollisionManager::GetInstance();
	collisionManager->AddCollider(pAttackCollider_.get());
	collisionManager->SetCollisionMask(static_cast<uint32_t>(ColliderTypeID::EnemyAttack), static_cast<uint32_t>(ColliderTypeID::Player), true);
}

void EnemyAttackState::Update(Enemy* enemy)
{
	// 時間の加算
	timer_ += Tako::FrameTimer::GetInstance()->GetDeltaTime();

	float t = timer_ / kAttackDuration_;
	if (t > 1.0f)
	{
		t = 1.0f;
	}

	// スムーズステップイージングの適用
	float easedT = t * t * (3.0f - 2.0f * t);

	// 敵の現在の基準方向を取得
	float baseYaw = enemy->GetTransform().rotate.y;
	
	// 右60度から左60度へ補間
	float startAngle = baseYaw + 3.14159265f / 3.0f;
	float endAngle = baseYaw - 3.14159265f / 3.0f;
	float currentAngle = startAngle + (endAngle - startAngle) * easedT;

	// 新しい位置と回転の計算
	Tako::Vector3 offset = { std::sin(currentAngle) * kColliderOffset, 0.0f, std::cos(currentAngle) * kColliderOffset };
	colliderTransform_.translate = enemy->GetPosition() + offset;
	colliderTransform_.translate.y += 0.5f; // 少し浮かす
	colliderTransform_.rotate = { 0.0f, currentAngle + 3.14159265f / 2.0f, 0.0f }; // 接線方向に向ける

	// モデルのトランスフォーム更新
	if (pAttackModel_)
	{
		pAttackModel_->SetTransform(colliderTransform_);
		pAttackModel_->Update();
	}
}

void EnemyAttackState::Exit(Enemy* enemy)
{
	// コライダーをマネージャーから削除
	auto collisionManager = Tako::CollisionManager::GetInstance();
	collisionManager->RemoveCollider(pAttackCollider_.get());
	pAttackCollider_ = nullptr;

	// モデルの破棄
	pAttackModel_ = nullptr;
}

void EnemyAttackState::Draw(Enemy* enemy)
{
	// 攻撃モデルの描画
	if (pAttackModel_)
	{
		pAttackModel_->Draw();
	}
}

void EnemyAttackState::DrawImGui(Enemy* enemy)
{

}

std::optional<EnemyStateType> EnemyAttackState::CheckTransition(Enemy* enemy)
{
	// 攻撃時間が経過したら追従状態に戻る
	if (timer_ >= kAttackDuration_)
	{
		return EnemyStateType::Chase;
	}
	return std::nullopt;
}