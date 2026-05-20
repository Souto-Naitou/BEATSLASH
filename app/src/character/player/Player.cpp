#include "Player.h"

#include <FrameTimer.h>
#include <type/ColliderTypeID.h>
#include <CollisionManager.h>

#ifdef _DEBUG
#include <debug/DebugRegisterer.h>
#include <imgui.h>
#endif // _DEBUG

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
    pModel_->SetTranslate({ 0.0f, 20.0f, 0.0f });            // 初期位置を設定

    // トランスフォームの初期化
    transform_ = pModel_->GetTransform();
    // コンポーネントの初期化
    this->InitializeComponents();
    const float modelBaseSize = 3.0f;
    // コライダーの初期化
    pCollider_ = std::make_unique<PlayerCollider>();
    pCollider_->SetSize(pModel_->GetScale() * modelBaseSize);
    pCollider_->SetTransform(&transform_);
    pCollider_->SetTypeID(static_cast<uint32_t>(ColliderTypeID::Player));
    pCollider_->SetPushBackCallback([this](const Tako::Vector3& pushBack)
                                    {
                                        transform_.translate += pushBack;
                                        if(pushBack.y > 0)
                                        {
                                            pMovement_->ResetVelocityY();
                                        }
                                        pModel_->SetTransform(transform_);
                                        pModel_->Update();
                                    });
    Tako::CollisionManager::GetInstance()->AddCollider(pCollider_.get());
    Tako::CollisionManager::GetInstance()->SetCollisionMask(static_cast<uint32_t>(ColliderTypeID::Player), static_cast<uint32_t>(ColliderTypeID::Terrain), true);
}

void Player::Finalize()
{
}

void Player::Update()
{
    //const float deltaTime = Tako::FrameTimer::GetInstance()->GetDeltaTime();
    const float deltaTime = 0.016f;

    // 入力の更新
    pInput_->Update();
    // 移動の更新
    pMovement_->ApplyFriction(kFrictionPower_);
    pMovement_->ApplyGravity(kMass_, deltaTime);
    pMovement_->Update(transform_, deltaTime);

    if (transform_.translate.y < 4.0f) // 地面に落ちないように最低限の高さを確保
    {
        //transform_.translate.y = 4.0f;
        //pMovement_->ResetVelocityY();
    }

    // モデルの更新
    pModel_->SetTransform(transform_);
    pModel_->Update();
}

void Player::Draw()
{
    pModel_->Draw();
}

void Player::RegisterCallbacks()
{
#ifdef _DEBUG

    kMovePower_.SetOnChange([this](const float newval)
                            {
                                pMovement_->SetMovePower(newval);
                            });

    kJumpPower_.SetOnChange([this](const float newval)
                            {
                                pMovement_->SetJumpPower(newval);
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
