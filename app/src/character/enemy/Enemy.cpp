#include "Enemy.h"
#include <CollisionManager.h>
#include <Input.h>
#include <imgui.h>
#include <DebugUIManager.h>
#include <type/ColliderTypeID.h>

Enemy::Enemy(const ICharacter* target)
	: pTarget_(target)
{}

void Enemy::Initialize()
{
	// モデルの初期化
	pModel_ = std::make_unique<Tako::Object3d>();
	pModel_->Initialize();
	pModel_->SetModel("white_cube.gltf");
	pModel_->SetMaterialColor({ 0,256,0,256 });
	pModel_->SetEnableLighting(true);
	pModel_->SetScale({ 1.0f, 1.0f, 1.0f });

	// トランスフォームの初期化
	transform_ = pModel_->GetTransform();
	transform_ = {
		.scale = { 1.0f, 1.0f, 1.0f },
		.rotate = { 0.0f, 0.0f, 0.0f },
		.translate = { 0.0f, 2.0f, 0.0f }
	};

	// コライダーの初期化
	pCollider_ = std::make_unique<EnemyCollider>();
	pCollider_->SetSize(pModel_->GetScale() * 3.0f);
	pCollider_->SetOwner(this);
	pCollider_->SetTypeID(static_cast<uint32_t>(ColliderTypeID::Enemy));
	pCollider_->SetTransform(&transform_);
	pCollider_->SetPushBackCallback([this](const Tako::Vector3& pushBack) {
		transform_.translate += pushBack;
									});

	// コライダーをマネージャーに登録
	auto collisionManager = Tako::CollisionManager::GetInstance();
	collisionManager->AddCollider(pCollider_.get());
	collisionManager->SetCollisionMask(static_cast<uint32_t>(ColliderTypeID::Enemy), static_cast<uint32_t>(ColliderTypeID::Player), true);
	collisionManager->SetCollisionMask(static_cast<uint32_t>(ColliderTypeID::Enemy), static_cast<uint32_t>(ColliderTypeID::Terrain), true);

	// デバッグUIの登録
	Tako::DebugUIManager::GetInstance()->RegisterGameObject("Enemy", [this]() { this->DrawImGui(); });

	// ステートの初期化。｛ 待機状態、追従 ｝
	stateMachine_.Initialize({ EnemyStateType::Idle, EnemyStateType::Chase }, this, pTarget_);
}

void Enemy::Update()
{
	// 状態の切り替え（デバッグ用）
	ChangeState();

	// ステートマシンの更新
	stateMachine_.Update();

	// トランスフォームの更新
	pModel_->SetTransform(transform_);
	pModel_->Update();
}

void Enemy::Draw()
{
	// モデルの描画
	pModel_->Draw();
}

void Enemy::ChangeState()
{
	if (Tako::Input::GetInstance()->PushKey(DIK_1))
	{
		stateMachine_.ChangeState(EnemyStateType::Idle);
		pModel_->SetMaterialColor({ 0,256,0,256 });
	}
	if (Tako::Input::GetInstance()->PushKey(DIK_2))
	{
		stateMachine_.ChangeState(EnemyStateType::Chase);
		pModel_->SetMaterialColor({ 256,0,0,256 });
	}
}

void Enemy::DrawImGui()
{
#ifdef _DEBUG
	ImGui::SliderFloat3("Position", &transform_.translate.x, -10.0f, 10.0f);
	ImGui::SliderFloat3("Rotation", &transform_.rotate.x, -3.14f, 3.14f);
	ImGui::SliderFloat3("Scale", &transform_.scale.x, 0.1f, 5.0f);
#endif
}
