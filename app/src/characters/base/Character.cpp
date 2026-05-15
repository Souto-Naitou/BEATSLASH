#include "Character.h"

void Character::Initialize(CharacterColliderID colliderID)
{
	// 3Dモデルの初期化
	model_ = std::make_unique<Tako::Object3d>();
	model_->Initialize();
	model_->SetModel("white_cube.gltf");
	model_->SetTransform(Tako::Transform());		// デフォルトのトランスフォームを設定
	model_->SetMaterialColor({ 1,1,1,1 });		// 白色のマテリアルカラーを設定
	model_->SetEnableLighting(true);				// ライティングを有効にする

	// トランスフォームを取得
	transform_ = model_->GetTransform();

	// コライダーの初期化
	collider_ = std::make_unique<CharacterCollider>();
	collider_->SetSize({ 1.0f, 1.0f, 1.0f });	// デフォルトのサイズを設定
	collider_->SetOwner(this);						// コライダーの所有者をこのキャラクターに設定
	collider_->SetTypeID(static_cast<uint32_t>(colliderID)); // コライダーの型IDを設定
	collider_->SetTransform(&transform_);			// コライダーのトランスフォームをキャラクターのトランスフォームに紐づける
}

void Character::Update()
{
	// トランスフォームの更新
	model_->SetTransform(transform_);
	model_->Update();
}

void Character::Draw()
{
	// モデルの描画
	model_->Draw();
}
