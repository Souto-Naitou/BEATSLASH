#include "stage.h"
#include <CollisionManager.h>

void Stage::Initialize(const std::string& filePath)
{
	// 3Dモデルの初期化
	model_ = std::make_unique<Tako::Object3d>();
	model_->Initialize();
	model_->SetModel("white_cube.gltf");
	model_->SetMaterialColor({ 1,1,1,1 });		// 白色のマテリアルカラーを設定
	model_->SetEnableLighting(true);				// ライティングを有効にする

    transform_.translate = { 0.0f, -0.5f, 0.0f };	// ステージを少し下に配置
    transform_.scale = { 10.0f, 1.0f, 10.0f };			// ステージを広くする
    transform_.rotate = { 0.0f, 0.0f, 0.0f };		// 回転なし
    model_->SetTransform(transform_);		// デフォルトのトランスフォームを設定
    	
    const float modelDefaultSize = 3.0f; 
    // コライダーの初期化
	collider_ = std::make_unique<Tako::AABBCollider>();
	collider_->SetSize({ 10.0f* modelDefaultSize, 1.0f * modelDefaultSize, 10.0f * modelDefaultSize}); // コライダーのサイズを設定
    collider_->SetOwner(this); // コライダーの所有者をステージに設定
    collider_->SetTypeID(200);// TODO : enum
    collider_->SetTransform(&transform_); // コライダーにステージのトランスフォームを設定

    // コライダーを衝突管理に登録
    Tako::CollisionManager::GetInstance()->AddCollider(collider_.get());
    Tako::CollisionManager::GetInstance()->SetCollisionMask(1, 200, true); // プレイヤーの型IDを1、ステージの型IDを200と仮定して衝突判定を有効化
}

void Stage::Update(float deltaTime)
{
    model_->SetTransform(transform_);	// トランスフォームをモデルに反映
    model_->Update();
}

void Stage::Draw()
{
    model_->Draw();
}
