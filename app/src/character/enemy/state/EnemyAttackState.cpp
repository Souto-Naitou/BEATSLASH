#include "EnemyAttackState.h"
#include "character/enemy/Enemy.h"
#include <type/ColliderTypeID.h>
#include <CollisionManager.h>
#include <FrameTimer.h>
#include <cmath>
#include <manager/BeatManager.h>
#include <EmitterManager.h>

#ifdef _DEBUG
#include <imgui.h>
#endif // _DEBUG

uint32_t EnemyAttackState::attackEffectCount_ = 0;
uint32_t EnemyAttackState::attackEffectModelCount_ = 0;

EnemyAttackState::EnemyAttackState(const ICharacter* target, Tako::EmitterManager* emitterManager)
	: pTarget_(target)
	, pEmitterManager_(emitterManager)
{
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

	// 攻撃エフェクトのロード
	effectName_ = "enemy_attack_" + std::to_string(attackEffectModelCount_++);
	pEmitterManager_->LoadPreset("enemy_attack", effectName_, pAttackModel_.get());
	pEmitterManager_->SetEmitterActive(effectName_, false); // 最初は非アクティブにしておく
}

EnemyAttackState::~EnemyAttackState()
{
	// コライダーをマネージャーから削除
	auto collisionManager = Tako::CollisionManager::GetInstance();
	if (pAttackCollider_)
	{
		collisionManager->RemoveCollider(pAttackCollider_.get());
	}
}

void EnemyAttackState::Enter(Enemy* enemy)
{
	// タイマーのリセット
	timer_ = 0.0f;

	const BeatClock* beatClock = enemy->GetBeatClock();
	if (beatClock)
	{
		startBeat_ = beatClock->GetCurrentBeat();
	}

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
	colliderTransform_.scale = { 1.8f, 0.1f, 0.3f }; // コライダーを薄長くする

	// コライダーをマネージャーに登録
	auto collisionManager = Tako::CollisionManager::GetInstance();
	collisionManager->AddCollider(pAttackCollider_.get());
	collisionManager->SetCollisionMask(static_cast<uint32_t>(ColliderTypeID::EnemyAttack), static_cast<uint32_t>(ColliderTypeID::Player), true);

	// 攻撃エフェクトの再生
	emitterTempName_ = effectName_ + "_temp_" + std::to_string(attackEffectCount_++);
	pEmitterManager_->CreateTemporaryEmitterFrom(effectName_, emitterTempName_, kAttackDuration_);
}

void EnemyAttackState::Update(Enemy* enemy)
{
	// 時間の加算
	timer_ += Tako::FrameTimer::GetInstance()->GetDeltaTime();

	const BeatClock* beatClock = enemy->GetBeatClock();
	float t = 0.0f;
	if (beatClock)
	{
		float currentBeat = beatClock->GetCurrentBeat();
		t = (currentBeat - startBeat_) / kAttackDurationInBeats;
	}
	else
	{
		t = timer_ / kAttackDuration_;
	}

	if (t > 1.0f)
	{
		t = 1.0f;
	}

	// イージングの適用(EaseInOutQuint)
	float easedT = t < 0.5f
		? 16.0f * t * t * t * t * t
		: 1.0f - std::pow(-2.0f * t + 2.0f, 5.0f) / 2.0f;

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

	// デバッグ用ログ出力（ピーク到達時に1回出力）
	static bool hasLoggedPeak = false;
	if (std::abs(t - 0.5f) < 0.02f)
	{
		if (!hasLoggedPeak)
		{
			OutputDebugStringA("--- Attack reached peak (t = 0.5) directly in front! ---\n");
			hasLoggedPeak = true;
		}
	}
	else if (t < 0.1f)
	{
		hasLoggedPeak = false; // 次の攻撃のためにリセット
	}
}

void EnemyAttackState::Exit(Enemy* enemy)
{
	// コライダーをマネージャーから削除
	auto collisionManager = Tako::CollisionManager::GetInstance();
	collisionManager->RemoveCollider(pAttackCollider_.get());

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
#ifdef _DEBUG
	const BeatClock* beatClock = enemy->GetBeatClock();
	if (beatClock)
	{
		float currentBeat = beatClock->GetCurrentBeat();
		float elapsed = currentBeat - startBeat_;
		ImGui::Text("Start Beat: %.4f", startBeat_);
		ImGui::Text("Current Beat: %.4f", currentBeat);
		ImGui::Text("Elapsed Beats: %.4f / %.2f", elapsed, kAttackDurationInBeats);
		ImGui::Text("Progress (t): %.4f (Peak is 0.5)", elapsed / kAttackDurationInBeats);
	}
#endif
}

std::optional<EnemyStateType> EnemyAttackState::CheckTransition(Enemy* enemy)
{
	const BeatClock* beatClock = enemy->GetBeatClock();
	if (beatClock)
	{
		float currentBeat = beatClock->GetCurrentBeat();
		if (currentBeat - startBeat_ >= kAttackDurationInBeats)
		{
			return EnemyStateType::Chase;
		}
	}
	else
	{
		// 攻撃時間が経過したら追従状態に戻る
		if (timer_ >= kAttackDuration_)
		{
			return EnemyStateType::Chase;
		}
	}
	return std::nullopt;
}