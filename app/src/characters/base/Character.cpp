#include "Character.h"

void Character::Initialize()
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

}

void Character::Update()
{
}

void Character::Draw()
{
}
