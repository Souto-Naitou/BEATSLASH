#include "Enemy.h"
#include <CollisionManager.h>
#include <Input.h>
#include <imgui.h>
#include <DebugUIManager.h>
#include <type/ColliderTypeID.h>
#include <FrameTimer.h>

Enemy::Enemy(const ICharacter* target)
	: pTarget_(target)
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
	pModel_->SetMaterialColor({ 0,256,0,256 });
	pModel_->SetEnableLighting(true);
	pModel_->SetScale({ 1.0f, 1.0f, 1.0f });
	pModel_->SetTranslate({ 0.0f,20.0f,0.0f });

	// トランスフォームの初期化
	transform_ = pModel_->GetTransform();

	// コライダーの初期化
	pCollider_ = std::make_unique<EnemyCollider>();
	pCollider_->SetSize(pModel_->GetScale() * 3.0f);
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
	collisionManager->SetCollisionMask(static_cast<uint32_t>(ColliderTypeID::Enemy), static_cast<uint32_t>(ColliderTypeID::Terrain), true);

	// デバッグUIの登録
	Tako::DebugUIManager::GetInstance()->RegisterGameObject("Enemy", [this]() { this->DrawImGui(); });

	// ステートの初期化。｛　待機状態、　｝
	stateMachine_.Initialize({ EnemyStateType::Idle, EnemyStateType::Chase, EnemyStateType::Attack }, this, pTarget_);

    pHp_ = std::make_unique<HPComponent>();
    // HPコンポーネントの初期化 TODO : 仮の値
    pHp_->Initialize(100);
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
		pModel_->SetMaterialColor({ 0,256,0,256 });
		return true;
	}
	if (Tako::Input::GetInstance()->PushKey(DIK_2))
	{
		stateMachine_.ChangeState(EnemyStateType::Chase);
		pModel_->SetMaterialColor({ 256,0,0,256 });
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
#endif
}
