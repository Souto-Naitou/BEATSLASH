#include "EnemyAttackState.h"
#include "character/enemy/Enemy.h"
#include <type/ColliderTypeID.h>
#include <CollisionManager.h>
#include <FrameTimer.h>
#include <cmath>
#include <numbers>
#include <manager/BeatClock.h>
#include <EmitterManager.h>
#include <sstream>
#include <Ozsound/audio/SoundEngine.h>

#ifdef _DEBUG
#include <imgui.h>
#endif // _DEBUG

uint32_t EnemyAttackState::nextInstanceId_ = 0;

EnemyAttackState::EnemyAttackState(const ICharacter* target, Tako::EmitterManager* emitterManager)
	: pTarget_(target)
	, pEmitterManager_(emitterManager)
{
	// コライダートランスフォームの初期化
	colliderTransform_.scale = { 1.8f, 0.1f, 0.3f };

	// 攻撃モデルの生成と初期化
	pAttackModel_ = std::make_unique<Tako::Object3d>();
	pAttackModel_->Initialize();
	pAttackModel_->SetModel("white_cube.gltf");
	pAttackModel_->SetMaterialColor({ 256, 128, 0, 256 }); // オレンジ色
	pAttackModel_->SetEnableLighting(true);
	pAttackModel_->SetTransform(colliderTransform_);
	pAttackModel_->Update();

	// 攻撃コライダーの生成と初期化
	pAttackCollider_ = std::make_unique<EnemyAttackCollider>(pEmitterManager_);
	pAttackCollider_->SetSize(pAttackModel_->GetScale() * 3.4f); // コライダーは少し大きめにする
	pAttackCollider_->SetTransform(&colliderTransform_);
	pAttackCollider_->SetTypeID(static_cast<uint32_t>(ColliderTypeID::EnemyAttack));
	pAttackCollider_->SetOwner(this);

	// エフェクト管理用配列のプリセット名を初期化
	effects_[static_cast<size_t>(AttackEffectType::Main)].presetName = "enemy_attack";
	effects_[static_cast<size_t>(AttackEffectType::Warning)].presetName = "enemy_attack_sign";
	effects_[static_cast<size_t>(AttackEffectType::WarningWeapon)].presetName = "enemy_attack_sign_weapon";

	// 静的カウンタから一意の文字列を生成
	std::string instanceId = std::to_string(nextInstanceId_++);

	for (auto& effect : effects_)
	{
		effect.baseName = effect.presetName + "_" + instanceId;
		pEmitterManager_->LoadPreset(effect.presetName, effect.baseName, pAttackModel_.get());
		pEmitterManager_->SetEmitterActive(effect.baseName, false); // 最初は非アクティブにしておく
	}
}

EnemyAttackState::~EnemyAttackState()
{
	// コライダーをマネージャーから削除
	auto collisionManager = Tako::CollisionManager::GetInstance();
	if (pAttackCollider_)
	{
		collisionManager->RemoveCollider(pAttackCollider_.get());
	}

	// エフェクトの削除
	if (pEmitterManager_)
	{
		for (auto& effect : effects_)
		{
			pEmitterManager_->RemoveEmitter(effect.baseName);
			if (!effect.tempName.empty())
			{
				pEmitterManager_->RemoveEmitter(effect.tempName);
			}
		}
	}
}

void EnemyAttackState::Enter(Enemy* enemy)
{
	// タイマーのリセット
	timer_ = 0.0f;
	isAttackStarted_ = false;

	const BeatClock* beatClock = enemy->GetBeatClock();
	if (beatClock)
	{
		startBeat_ = beatClock->GetCurrentBeat();
	}

	// 敵の現在の回転を取得
	float baseYaw = enemy->GetTransform().rotate.y;

	// 開始角度は右に60度
	float startAngle = baseYaw + std::numbers::pi_v<float> / 3.0f;

	// コライダートランスフォームの初期化
	colliderTransform_ = enemy->GetTransform();
	Tako::Vector3 forward = { std::sin(startAngle), 0.0f, std::cos(startAngle) };
	colliderTransform_.translate += forward * kColliderOffset;
	colliderTransform_.translate.y -= 0.7f; // 少し下げる
	colliderTransform_.rotate = { 0.0f, startAngle + std::numbers::pi_v<float> / 2.0f, 0.0f }; // 接線方向に向ける
	colliderTransform_.scale = { 1.8f, 0.1f, 0.3f }; // コライダーを薄長くする

	// コライダーは最初は非アクティブに設定
	pAttackCollider_->SetActive(false);

	// コライダーをマネージャーに登録
	auto collisionManager = Tako::CollisionManager::GetInstance();
	collisionManager->AddCollider(pAttackCollider_.get());
	collisionManager->SetCollisionMask(static_cast<uint32_t>(ColliderTypeID::EnemyAttack), static_cast<uint32_t>(ColliderTypeID::Player), true);

	// 警告表示用の初期カラーを設定（赤色、半透明）
	pAttackModel_->SetMaterialColor({ 256.0f, 0.0f, 0.0f, 120.0f });
	pAttackModel_->SetTransparent(true);
	pAttackModel_->SetTransform(colliderTransform_);
	pAttackModel_->Update();

	// 予備動作エフェクトの再生
	auto& warningEffect = effects_[static_cast<size_t>(AttackEffectType::Warning)];
	warningEffect.tempName = warningEffect.baseName + "_temp_" + std::to_string(playCount_);
	pEmitterManager_->CreateTemporaryEmitterFrom(warningEffect.baseName, warningEffect.tempName, kWarningDuration_);
	pEmitterManager_->SetEmitterPosition(warningEffect.tempName, enemy->GetPosition());
	pEmitterManager_->GetEmitterByName(warningEffect.tempName)->BindTargetPosition(&enemy->GetPosition());

	// 予備動作エフェクト（武器）の再生
	auto& warningWeaponEffect = effects_[static_cast<size_t>(AttackEffectType::WarningWeapon)];
	warningWeaponEffect.tempName = warningWeaponEffect.baseName + "_temp_" + std::to_string(playCount_++);
	pEmitterManager_->CreateTemporaryEmitterFrom(warningWeaponEffect.baseName, warningWeaponEffect.tempName, kWarningDuration_);
	pEmitterManager_->SetEmitterPosition(warningWeaponEffect.tempName, colliderTransform_.translate);

	// 予備動作のSEを流す
	ozSound::SoundEngine::GetInstance()->PostEvent("play_se_enemy_attack_sign");
}

void EnemyAttackState::Update(Enemy* enemy)
{
	// 時間の加算
	timer_ += Tako::FrameTimer::GetInstance()->GetDeltaTime();

	const BeatClock* beatClock = enemy->GetBeatClock();
	bool isWarning = true;
	float attackProgress = 0.0f;
	if (beatClock)
	{
		float currentBeat = beatClock->GetCurrentBeat();
		float elapsedBeats = currentBeat - startBeat_;
		if (elapsedBeats < kWarningDurationInBeats)
		{
			isWarning = true;
		}
		else
		{
			isWarning = false;
			attackProgress = (elapsedBeats - kWarningDurationInBeats) / kAttackDurationInBeats;
		}
	}
	else
	{
		if (timer_ < kWarningDuration_)
		{
			isWarning = true;
		}
		else
		{
			isWarning = false;
			attackProgress = (timer_ - kWarningDuration_) / kAttackDuration_;
		}
	}

	if (attackProgress > 1.0f)
	{
		attackProgress = 1.0f;
	}

	if (isWarning)
	{
		// 警告フェーズ：開始角度で赤色点滅
		float flashAngle = timer_ * 15.0f;
		float alpha = 120.0f + 80.0f * std::sin(flashAngle);
		pAttackModel_->SetMaterialColor({ 256.0f, 0.0f, 0.0f, alpha });
		pAttackModel_->SetTransparent(true);

		float baseYaw = enemy->GetTransform().rotate.y;
		float startAngle = baseYaw + std::numbers::pi_v<float> / 3.0f;

		Tako::Vector3 offset = { std::sin(startAngle) * kColliderOffset, 0.0f, std::cos(startAngle) * kColliderOffset };
		colliderTransform_.translate = enemy->GetPosition() + offset;
		colliderTransform_.translate.y -= 0.7f;
		colliderTransform_.rotate = { 0.0f, startAngle + std::numbers::pi_v<float> / 2.0f, 0.0f };
	}
	else
	{
		// 攻撃開始の瞬間処理
		if (!isAttackStarted_)
		{
			isAttackStarted_ = true;
			pAttackCollider_->SetActive(true);
			pAttackModel_->SetMaterialColor({ 256.0f, 128.0f, 0.0f, 256.0f }); // オレンジ色
			pAttackModel_->SetTransparent(false);

			// 攻撃エフェクトの再生
			auto& mainEffect = effects_[static_cast<size_t>(AttackEffectType::Main)];
			mainEffect.tempName = mainEffect.baseName + "_temp_" + std::to_string(playCount_++);
			pEmitterManager_->CreateTemporaryEmitterFrom(mainEffect.baseName, mainEffect.tempName, kAttackDuration_);

			// SEを流す
			ozSound::SoundEngine::GetInstance()->PostEvent("play_se_enemy_attack");
		}

		// 攻撃フェーズ：イージングを適用したスイング
		float easedT = attackProgress < 0.5f
			? 16.0f * attackProgress * attackProgress * attackProgress * attackProgress * attackProgress
			: 1.0f - std::pow(-2.0f * attackProgress + 2.0f, 5.0f) / 2.0f;

		float baseYaw = enemy->GetTransform().rotate.y;
		float startAngle = baseYaw + std::numbers::pi_v<float> / 3.0f;
		float endAngle = baseYaw - std::numbers::pi_v<float> / 3.0f;
		float currentAngle = startAngle + (endAngle - startAngle) * easedT;

		Tako::Vector3 offset = { std::sin(currentAngle) * kColliderOffset, 0.0f, std::cos(currentAngle) * kColliderOffset };
		colliderTransform_.translate = enemy->GetPosition() + offset;
		colliderTransform_.translate.y -= 0.7f;
		colliderTransform_.rotate = { 0.0f, currentAngle + std::numbers::pi_v<float> / 2.0f, 0.0f };
	}

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
		ImGui::Text("Elapsed Beats: %.4f / %.2f", elapsed, kWarningDurationInBeats + kAttackDurationInBeats);
		ImGui::Text("Is Warning: %s", (elapsed < kWarningDurationInBeats) ? "Yes" : "No");
		if (elapsed >= kWarningDurationInBeats)
		{
			ImGui::Text("Attack Progress: %.4f", (elapsed - kWarningDurationInBeats) / kAttackDurationInBeats);
		}
	}
#endif
}

std::optional<EnemyStateType> EnemyAttackState::CheckTransition(Enemy* enemy)
{
	const BeatClock* beatClock = enemy->GetBeatClock();
	if (beatClock)
	{
		float currentBeat = beatClock->GetCurrentBeat();
		if (currentBeat - startBeat_ >= kWarningDurationInBeats + kAttackDurationInBeats)
		{
			return EnemyStateType::Chase;
		}
	}
	else
	{
		// 予備動作時間＋攻撃時間が経過したら追従状態に戻る
		if (timer_ >= kWarningDuration_ + kAttackDuration_)
		{
			return EnemyStateType::Chase;
		}
	}
	return std::nullopt;
}