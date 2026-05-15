#include "Player.h"



void Player::Initialize()
{
    // 3Dモデルの初期化
    pModel_ = std::make_unique<Tako::Object3d>();
    pModel_->Initialize();
    pModel_->SetModel("white_cube.gltf");
    pModel_->SetTransform(Tako::Transform());               // デフォルトのトランスフォームを設定
    pModel_->SetMaterialColor({ 1.0f, 1.0f, 1.0f, 1.0f });  // 白色のマテリアルカラーを設定
    pModel_->SetEnableLighting(true);                       // ライティングを有効にする

    transform_ = pModel_->GetTransform();
}

void Player::Update()
{
    pMovement_->Update(transform_, 1.0f / 60.0f);
}

void Player::Draw()
{
    pModel_->Draw();
}

void Player::InitializeComponents()
{
    pInput_ = std::make_unique<PlayerInput>();
    pInput_->Initialize();
    pMovement_ = std::make_unique<PlayerMovement>(pInput_.get());
}
