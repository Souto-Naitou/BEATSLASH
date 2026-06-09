#include "Enemy.h"
#include <CollisionManager.h>
#include <Input.h>
#include <type/ColliderTypeID.h>
#include <FrameTimer.h>
#include <manager/BeatManager.h>
#include <numbers>

#ifdef _DEBUG
#include <imgui.h>
#endif // _DEBUG

Enemy::Enemy(const ICharacter* target, const BeatClock* beatClock)
	: pTarget_(target)
	, pBeatClock_(beatClock)
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
	pCollider_ = std::make_unique<EnemyCollider>();
	pCollider_->SetSize(pModel_->GetScale() * kColliderScaleMultiplier);
	pCollider_->SetOwner(this);
	pCollider_->SetTypeID(static_cast<uint32_t>(ColliderTypeID::Enemy));
	pCollider_->SetTransform(&transform_);
	pCollider_->SetPushBackCallback([this](const Tako::Vector3& pushBack) {
		transform_.translate += pushBack;
		pModel_->SetTransform(transform_);
		pModel_->Update();
									});

	// コライダーをマネージャーに登録
	auto collisionManager = Tako::CollisionManager::GetInstance();
	collisionManager->AddCollider(pCollider_.get());
	collisionManager->SetCollisionMask(static_cast<uint32_t>(ColliderTypeID::Enemy), static_cast<uint32_t>(ColliderTypeID::Player), true);
	collisionManager->SetCollisionMask(static_cast<uint32_t>(ColliderTypeID::Enemy), static_cast<uint32_t>(ColliderTypeID::Enemy), true);
	collisionManager->SetCollisionMask(static_cast<uint32_t>(ColliderTypeID::Enemy), static_cast<uint32_t>(ColliderTypeID::Terrain), true);

	// ステートの初期化。｛　待機状態、　｝
	stateMachine_.Initialize({ EnemyStateType::Idle, EnemyStateType::Chase, EnemyStateType::Attack }, this, pTarget_);

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
		// ステートマシンの更新
		stateMachine_.Update();
	}

	// 拍同期の拡縮アニメーションを適用する
	UpdateBeatAnimation();

	// 重力の適用
	transform_.translate.y += kGravity * Tako::FrameTimer::GetInstance()->GetDeltaTime();

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
