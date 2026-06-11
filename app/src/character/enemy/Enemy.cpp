#include "Enemy.h"
#include <CollisionManager.h>
#include <Input.h>
#ifdef _DEBUG
#include <imgui.h>
#include <DebugUIManager.h>
#endif
#include <type/ColliderTypeID.h>
#include <FrameTimer.h>
#include <manager/BeatClock.h>
#include <numbers>

#ifdef _DEBUG
#include <imgui.h>
#endif // _DEBUG

Enemy::Enemy(const ICharacter* target, const BeatClock* beatClock, Tako::EmitterManager* emitterManager)
	: pTarget_(target)
	, pBeatClock_(beatClock)
	, pEmitterManager_(emitterManager)
{}

Enemy::~Enemy()
{
	// コライダーをマネージャーから削除
	Tako::CollisionManager::GetInstance()->RemoveCollider(pCollider_.get());
}

void Enemy::Initialize()
{
	// モデルの初期化
	pModel_ = std::make_unique<Tako::Object3d>();
	pModel_->Initialize();
	pModel_->SetModel("white_cube.gltf");
	pModel_->SetMaterialColor(kInitialMaterialColor);
	pModel_->SetEnableLighting(true);
	pModel_->SetTranslate(kInitialTranslate);
	pModel_->SetScale(kInitialScale);

	// トランスフォームの初期化
	transform_ = pModel_->GetTransform();

	// コライダーの初期化
	pCollider_ = std::make_unique<EnemyCollider>(pEmitterManager_);
	pCollider_->SetSize(pModel_->GetScale() * kColliderScaleMultiplier);
	pCollider_->SetOwner(this);
	pCollider_->SetTypeID(static_cast<uint32_t>(ColliderTypeID::Enemy));
	pCollider_->SetTransform(&transform_);
	pCollider_->SetPushBackCallback([this](const Tako::Vector3& pushBack) {
		transform_.translate += pushBack;
		pModel_->SetTransform(transform_);
		pModel_->Update();
									});

	// HPコンポーネントの生成と初期化
	pHp_ = std::make_unique<HPComponent>();
	pHp_->Initialize(kInitialHP);
	// コライダーにHPコンポーネントをセット
	pCollider_->SetHPComponent(pHp_.get());
}

void Enemy::Update()
{
	// 状態の切り替え（デバッグ用）。切り替わった場合は自動遷移をスキップ
	bool isManualChanged = ChangeState();

	if (!isManualChanged)
	{
		// HPが0以下かつ現在のステートがDeadでなければ、死亡状態に遷移する
		if (pHp_ && !pHp_->IsAlive() && stateMachine_.GetCurrentState() != EnemyStateType::Dead)
		{
			stateMachine_.ChangeState(EnemyStateType::Dead);
		}

		// ステートマシンの更新
		stateMachine_.Update();
	}

	// スポーン状態および死亡状態の場合は拡縮アニメーションを適用しない
	if (stateMachine_.GetCurrentState() != EnemyStateType::Spawn && stateMachine_.GetCurrentState() != EnemyStateType::Dead)
	{
		// 拍同期の拡縮アニメーションを適用する
		UpdateBeatAnimation();
	}

	// スポーン状態以外は重力を適用する
	if (stateMachine_.GetCurrentState() != EnemyStateType::Spawn)
	{
		// 重力の適用
		transform_.translate.y += kGravity * 0.016f; // 仮の重力値とフレーム時間
	}

	// トランスフォームの更新
	pModel_->SetTransform(transform_);
	pModel_->Update();
}

void Enemy::Draw()
{
	// モデルの描画
	pModel_->Draw();

	// ステートの描画（攻撃エフェクトなどの描画）
	stateMachine_.Draw();
}

bool Enemy::ChangeState()
{
	if (Tako::Input::GetInstance()->PushKey(DIK_1))
	{
		stateMachine_.ChangeState(EnemyStateType::Idle);
		pModel_->SetMaterialColor(kInitialMaterialColor);
		return true;
	}
	if (Tako::Input::GetInstance()->PushKey(DIK_2))
	{
		stateMachine_.ChangeState(EnemyStateType::Chase);
		pModel_->SetMaterialColor(kChaseStateMaterialColor);
		return true;
	}
	return false;
}

void Enemy::DrawImGui()
{
#ifdef _DEBUG
	ImGui::SeparatorText("Transform");
	ImGui::SliderFloat3("Position", &transform_.translate.x, -10.0f, 10.0f);
	ImGui::SliderFloat3("Rotation", &transform_.rotate.x, -3.14f, 3.14f);
	ImGui::SliderFloat3("Scale", &transform_.scale.x, 0.1f, 5.0f);
	ImGui::SeparatorText("State");
	ImGui::Text("Current State: %s", GetStateName(stateMachine_.GetCurrentState()).c_str());

	// ステートのデバッグUIを描画
	stateMachine_.DrawImGui();
#endif
}

void Enemy::UpdateBeatAnimation()
{
	if (pBeatClock_)
	{
		// 拍に同期したコサイン波で拡縮する
		float beat = pBeatClock_->GetCurrentBeat();
		float scale = baseScale_ + scaleAmplitude_ * std::cos(beat * 2.0f * std::numbers::pi_v<float>);
		SetScale({ scale, scale, scale });
	}
	else
	{
		// タイマーの加算
		timer_ += Tako::FrameTimer::GetInstance()->GetDeltaTime();

		// 経過時間に基づいたサイン波
		float scale = baseScale_ + scaleAmplitude_ * std::sin(timer_ * scaleSpeed_);
		SetScale({ scale, scale, scale });
	}
}

void Enemy::InitializeStateMachine()
{
	// ステートの初期化。｛　スポーン状態、待機状態、追従状態、攻撃状態、死亡状態　｝
	stateMachine_.Initialize({ EnemyStateType::Spawn, EnemyStateType::Idle, EnemyStateType::Chase, EnemyStateType::Attack, EnemyStateType::Dead }, this, pTarget_, pEmitterManager_);
}

void Enemy::EnableCollider()
{
	// コライダーをマネージャーに登録
	auto collisionManager = Tako::CollisionManager::GetInstance();
	collisionManager->AddCollider(pCollider_.get());
	collisionManager->SetCollisionMask(static_cast<uint32_t>(ColliderTypeID::Enemy), static_cast<uint32_t>(ColliderTypeID::Player), true);
	collisionManager->SetCollisionMask(static_cast<uint32_t>(ColliderTypeID::Enemy), static_cast<uint32_t>(ColliderTypeID::Enemy), true);
	collisionManager->SetCollisionMask(static_cast<uint32_t>(ColliderTypeID::Enemy), static_cast<uint32_t>(ColliderTypeID::Terrain), true);

}

void Enemy::DisableCollider()
{
	if (pCollider_)
	{
		Tako::CollisionManager::GetInstance()->RemoveCollider(pCollider_.get());
	}
}
