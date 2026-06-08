#include "Player.h"

#include <FrameTimer.h>
#include <type/ColliderTypeID.h>
#include <CollisionManager.h>
#include <math/VectorMath.h>
#include <ozSound/audio/SoundEngine.h>
#include <utility/DeltaTimeManager.h>
#include <Model.h>

#ifdef _DEBUG
#include <debug/DebugRegisterer.h>
#include <imgui.h>
#endif // _DEBUG

void Player::Initialize()
{
    this->RegisterCallbacks();

    // オーバードライブの初期化
    pOverdrive_ = std::make_unique<Overdrive>(&comboBuffSystem_);
    pUpTempo_ = std::make_unique<UpTempo>(beatClock_);

    // 攻撃ヒット判定を Overdrive に通知するコールバックを登録
    attackRepository_.SetOnJudgeCallback([this](JudgeResult j)
    {
        pOverdrive_->OnJudge(j);
    });

    // 3Dモデルの初期化
    pModel_ = std::make_unique<Tako::Object3d>();
    pModel_->Initialize();
    pModel_->SetModel("PlayerAttack.gltf");
    pModel_->SetTransform(Tako::Transform());               // デフォルトのトランスフォームを設定
    pModel_->SetMaterialColor({ 0.1f, 0.8f, 0.1f, 1.0f });  // 緑色のマテリアルカラーを設定
    pModel_->SetEnableLighting(true);                       // ライティングを有効にする
    pModel_->SetScale({ 1.0f, 1.0f, 1.0f });                // スケールを設定
    pModel_->SetTranslate({ 0.0f, 8.0f, 0.0f });            // 初期位置を設定
    auto trueModel = pModel_->GetModel();
    trueModel->SetAnimation("PlayerAttack");
    trueModel->SetAnimationLoop("PlayerAttack", true);


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
        if (pushBack.y > 0)
        {
            pMovement_->ResetVelocityY();
        }
        pModel_->SetTransform(transform_);
        pModel_->Update();
    });

    auto colManeger = Tako::CollisionManager::GetInstance();

    colManeger->AddCollider(pCollider_.get());
    colManeger->SetCollisionMask(static_cast<uint32_t>(ColliderTypeID::Player), static_cast<uint32_t>(ColliderTypeID::Terrain), true);
    colManeger->SetCollisionMask(static_cast<uint32_t>(ColliderTypeID::Player), static_cast<uint32_t>(ColliderTypeID::StageTransitionEvent), true);
    colManeger->SetCollisionMask(static_cast<uint32_t>(ColliderTypeID::Player), static_cast<uint32_t>(ColliderTypeID::Enemy), true);
    colManeger->SetCollisionMask(static_cast<uint32_t>(ColliderTypeID::PlayerAttack), static_cast<uint32_t>(ColliderTypeID::Enemy), true);

}

void Player::Finalize()
{
}

void Player::Update()
{
    //const float deltaTime = Tako::FrameTimer::GetInstance()->GetDeltaTime();
    const float deltaTime = DeltaTimeManager::GetInstance()->GetDeltaTime(DeltaTimeChannelReserved::Game);

    // 入力の更新
    pInput_->Update();
    auto& inputCommand = pInput_->GetCommand();
    // 移動の更新
    pMovement_->ApplyFriction(kFrictionPower_);
    pMovement_->ApplyGravity(kMass_, deltaTime);
    pMovement_->Update(transform_, deltaTime);

    /// 移動しているときだけ向きを変える
    /// TODO: クラスに分離する
    if (inputCommand.move.LengthSquared() > 0.01f)
    {
        float yawCamera = followCamera_.GetRotation().y;
        float angle = VectorToAngle(inputCommand.move);
        transform_.rotate.y = angle + yawCamera;
        directionAtackSpawning = pMovement_->GetMoveDirection();
    }
    /// 攻撃時
    if (pAttackTrigger_->ShouldAttack(inputCommand))
    {
        Tako::Vector3 targetPos = transform_.translate;
        targetPos += directionAtackSpawning * 3.0f; // 攻撃の発生位置をプレイヤーの前方に設定
        attackRepository_.CreatePlayerAttack(targetPos);
        ozSound::SoundEngine::GetInstance()->PostEvent("play_player_attack");
    }

    if (inputCommand.isOverdriveTriggered)
    {
        pOverdrive_->Activate();
    }
    if (inputCommand.isUpTempoTriggered)
    {
        pUpTempo_->Activate();
    }

    pOverdrive_->Update();
    pUpTempo_->Update();

    pAttackTrigger_->UpdateCooldown(deltaTime);

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

void Player::Respawn(const Tako::Transform& spawnTransform)
{
    transform_.translate = spawnTransform.translate;
    transform_.rotate = spawnTransform.rotate;

    pModel_->SetTransform(transform_);
    pMovement_->ResetVelocity();
}

void Player::InitializeComponents()
{
    pInput_ = std::make_unique<PlayerInput>();
    pInput_->Initialize();
    pMovement_ = std::make_unique<PlayerMovement>(pInput_.get(), followCamera_);
    pMovement_->SetMovePower(kMovePower_);
    pMovement_->SetJumpPower(kJumpPower_);

    pAttackTrigger_ = std::make_unique<PlayerAttackTrigger>();
    pAttackTrigger_->CalculateCooldownTime(beatClock_.GetSecondsPerBeat());
}
