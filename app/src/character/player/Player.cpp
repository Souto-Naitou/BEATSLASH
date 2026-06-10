#include "Player.h"

#include <type/ColliderTypeID.h>
#include <CollisionManager.h>
#include <math/VectorMath.h>
#include <ozSound/audio/SoundEngine.h>
#include <utility/DeltaTimeManager.h>
#include <common/PlayerAttackRequest.h>
#include <debug/DebugRegisterer.h>
#include <Model.h>
#include <Vec3Func.h>
#include <FrameTimer.h>

// ステート
#include "state/PlayerStateContext.h"
#include "state/PlayerStateIdle.h"

#ifdef _DEBUG
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
    // アニメーションのループ設定
    auto trueModel = pModel_->GetModel();
    trueModel->SetAnimationLoop("PlayerAttack", false);

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
            pMovement_->SetGrounded(true);
        }
        pModel_->SetTransform(transform_);
    });
    pCollider_->SetComboSystem(&comboBuffSystem_);

    auto colManeger = Tako::CollisionManager::GetInstance();
    colManeger->AddCollider(pCollider_.get());
    colManeger->SetCollisionMask(static_cast<uint32_t>(ColliderTypeID::Player), static_cast<uint32_t>(ColliderTypeID::Terrain), true);
    colManeger->SetCollisionMask(static_cast<uint32_t>(ColliderTypeID::Player), static_cast<uint32_t>(ColliderTypeID::StageTransitionEvent), true);
    colManeger->SetCollisionMask(static_cast<uint32_t>(ColliderTypeID::Player), static_cast<uint32_t>(ColliderTypeID::Enemy), true);
    colManeger->SetCollisionMask(static_cast<uint32_t>(ColliderTypeID::PlayerAttack), static_cast<uint32_t>(ColliderTypeID::Enemy), true);

    // ステートマシンの初期化
    pStateMachine_ = std::make_unique<StateMachine<PlayerStateContext>>();

    pHPComponent_ = std::make_unique<HPComponent>(); 
    pHPComponent_->Initialize(100);// TODO : 仮の最大HPを100に設定
    pCollider_->SetHPComponent(pHPComponent_.get());

    // 初期状態を設定（例: IdleState）
    auto ctx = PlayerStateContext{ *this };
    pStateMachine_->ChangeState(states_[static_cast<size_t>(State::Idle)].get(), ctx);

    DebugRegister("PlayerModel", &Tako::Object3d::DrawImGui, pModel_.get());
}

void Player::Finalize()
{
    DebugUnregister("PlayerModel");
}

void Player::Update()
{
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
    }
    /// 攻撃時
    if (pAttackTrigger_->ShouldAttack(inputCommand))
    {
        PlayerAttackRequest request
        {
            .position = jointPosition_,
            .model = *pModel_->GetModel()
        };
        attackRepository_.CreatePlayerAttack(request);
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

    /// ジョイントのワールド座標を取得して保存
    auto matrix = pModel_->GetModel()->GetJointWorldMatrix("Tip", pModel_->GetWorldMatrix());
    jointPosition_ = { matrix.m[3][0], matrix.m[3][1], matrix.m[3][2] };
    Tako::Vector3 s, r;
    Tako::Mat4x4::Decompose(matrix, jointPosition_, r, s);
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

void Player::InitializeStates()
{
    states_[static_cast<size_t>(State::Idle)] = std::make_unique<PlayerStateIdle>();
}
