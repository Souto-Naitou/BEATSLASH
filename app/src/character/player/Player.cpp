#include "Player.h"
#include <imgui.h>

#include <debug/DebugRegisterer.h>

void Player::Initialize()
{
    this->RegisterCallbacks();

    // 3Dモデルの初期化
    pModel_ = std::make_unique<Tako::Object3d>();
    pModel_->Initialize();
    pModel_->SetModel("white_cube.gltf");
    pModel_->SetTransform(Tako::Transform());               // デフォルトのトランスフォームを設定
    pModel_->SetMaterialColor({ 0.1f, 0.8f, 0.1f, 1.0f });  // 緑色のマテリアルカラーを設定
    pModel_->SetEnableLighting(true);                       // ライティングを有効にする
    pModel_->SetScale({ 0.75f, 2.0f, 0.75f });              // スケールを設定
    pModel_->SetTranslate({ 0.0f, 4.0f, 0.0f });            // 初期位置を設定

    // トランスフォームの初期化
    transform_ = pModel_->GetTransform();
    // コンポーネントの初期化
    this->InitializeComponents();
}

void Player::Finalize()
{
}

void Player::Update()
{
    // 入力の更新
    pInput_->Update();
    // 移動の更新
    pMovement_->ApplyFriction(kFrictionPower_);
    pMovement_->Update(transform_, 1.0f / 60.0f);
    // モデルの更新
    pModel_->SetTransform(transform_);
    pModel_->Update();

    //testTransform_->scale = transform_.scale;
    //testTransform_ = transform_;
}

void Player::Draw()
{
    pModel_->Draw();
}

void Player::RegisterCallbacks()
{
#ifdef _DEBUG

    kMovePower_.SetOnChange([this](const float newval) {
        pMovement_->SetMovePower(newval);
    });

    kJumpPower_.SetOnChange([this](const float newval) {
        pMovement_->SetJumpPower(newval);
    });

    kGravity_.SetOnChange([this](const float newval) {
        // 重力の変更は、プレイヤーの移動処理に直接影響を与えるため、ここで反映させる必要があります。
        // 例えば、PlayerMovement クラスに SetGravity メソッドがある場合は、以下のように呼び出します。
        // pMovement_->SetGravity(newval);
    });

#endif // _DEBUG
}

void Player::InitializeComponents()
{
    pInput_ = std::make_unique<PlayerInput>();
    pInput_->Initialize();
    pMovement_ = std::make_unique<PlayerMovement>(pInput_.get());
    pMovement_->SetMovePower(kMovePower_);
    pMovement_->SetJumpPower(kJumpPower_);
}
