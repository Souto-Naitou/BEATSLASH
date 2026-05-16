#include "Enemy.h"
#include <CollisionManager.h>

void Enemy::Initialize()
{
	// モデルの初期化
	pModel_ = std::make_unique<Tako::Object3d>();
	pModel_->Initialize();
	pModel_->SetModel("white_cube.gltf");
	pModel_->SetTranslate({ 0.0f, 0.0f, 0.0f });
	pModel_->SetMaterialColor({ 1,1,1,1 });
	pModel_->SetEnableLighting(true);

	// トランスフォームの初期化
	transform_ = pModel_->GetTransform();

	// コライダーの初期化
	pCollider_ = std::make_unique<CharacterCollider>();
	pCollider_->SetSize({ 1.0f, 1.0f, 1.0f });
	pCollider_->SetOwner(this);
	pCollider_->SetTypeID(static_cast<uint32_t>(CharacterColliderID::Enemy));
	pCollider_->SetTransform(&transform_);

	// コライダーをマネージャーに登録
	//CollisionManager::GetInstance()->AddCollider(pCollider_.get());

	// ステートの初期化。｛　待機状態、　｝
	stateMachine_.Initialize({ EnemyStateType::Idle }, this);
}

void Enemy::Update()
{
	// ステートマシンの更新
	stateMachine_.Update();
}

void Enemy::Draw()
{
	// モデルの描画
	pModel_->Draw();
}
