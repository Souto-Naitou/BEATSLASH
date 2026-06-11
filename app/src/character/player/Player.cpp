#include "Player.h"

#include <type/ColliderTypeID.h>
#include <CollisionManager.h>
#include <math/VectorMath.h>
#include <ozSound/audio/SoundEngine.h>
#include <utility/DeltaTimeManager.h>
#include <common/PlayerAttackRequest.h>
#include <common/AnimationNames.h>
#include <debug/DebugRegisterer.h>
#include <Model.h>

// ステート
#include "state/PlayerStateContext.h"
#include "state/PlayerStateIdle.h"

void Player::Initialize()
{
    // コールバックの登録
    this->RegisterCallbacks();

    // スキルの初期化
    this->InitializeSkills();

    // 攻撃ヒット判定を Overdrive に通知するコールバックを登録
    attackRepository_.SetOnJudgeCallback([this](JudgeResult j)
    {
        pOverdrive_->OnJudge(j);
    });

    this->InitializeObject3d();

    // トランスフォームの初期化
    transform_ = pModel_->GetTransform();

    // コンポーネントの初期化
    this->InitializeComponents();

    // コライダーの初期化
    this->InitializeCollider();

    // ステートの初期化
    this->InitializeStates();

    // モデルデバッグ用のコールバック登録
    DebugRegister("PlayerModel", &Tako::Object3d::DrawImGui, pModel_.get());
}

void Player::Finalize()
{
    // デバッグコールバックの登録解除
    DebugUnregister("PlayerModel");
    Tako::CollisionManager::GetInstance()->RemoveCollider(pCollider_.get());
}

void Player::Update()
{
    const float deltaTime = DeltaTimeManager::GetInstance()->GetDeltaTime(DeltaTimeChannelReserved::Game);

    /// 入力の更新
    pInput_->Update();
    auto& inputCommand = pInput_->GetCommand();

    /// 移動の更新
    pMovement_->ApplyFriction(kFrictionPower_);
    pMovement_->ApplyGravity(kMass_, deltaTime);
    pMovement_->Update(transform_, deltaTime);
    /// 移動力の更新：攻撃中は移動力を下げる
    if (attackRepository_.IsEmpty())
    {
        pMovement_->SetMovePower(kMovePower_);
    }
    else
    {
        pMovement_->SetMovePower(kMovePowerOnAttack_);
    }

    /// コライダーの座標の更新
    colliderTransform_ = transform_;
    colliderTransform_.translate.y += kColliderSize_->y * 0.5f;

    /// 移動しているときだけ向きを変える
    if (inputCommand.move.LengthSquared() > 0.01f)
    {
        float yawCamera = followCamera_.GetRotation().y;
        float angle = VectorToAngle(inputCommand.move);
        transform_.rotate.y = angle + yawCamera;
    }

    /// 攻撃キーがトリガーされたとき
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

    // スキルの更新
    this->UpdateSkills(inputCommand);

    // 攻撃トリガーのクールダウン更新
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

void Player::Respawn(const Tako::Transform& spawnTransform)
{
    transform_.translate = spawnTransform.translate;
    transform_.rotate = spawnTransform.rotate;

    pModel_->SetTransform(transform_);
    pMovement_->ResetVelocity();
}

#pragma region Initialize Methods

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
    pMovement_ = std::make_unique<PlayerMovement>(pInput_.get(), followCamera_);
    pMovement_->SetMovePower(kMovePower_);
    pMovement_->SetJumpPower(kJumpPower_);

    pAttackTrigger_ = std::make_unique<PlayerAttackTrigger>();
    pAttackTrigger_->CalculateCooldownTime(beatClock_.GetSecondsPerBeat());

    pHPComponent_ = std::make_unique<HPComponent>();
    pHPComponent_->Initialize(100);// TODO : 仮の最大HPを100に設定

    pParryHistory_ = std::make_unique<ParryHistory>();
    pParryJudgement_ = std::make_unique<ParryJudgement>(*pParryHistory_);
    pParryPresentation_ = std::make_unique<ParryPresentation>(particleEmitter_);
}

void Player::InitializeStates()
{
    states_[static_cast<size_t>(State::Idle)] = std::make_unique<PlayerStateIdle>();

    // ステートマシンの初期化
    pStateMachine_ = std::make_unique<StateMachine<PlayerStateContext>>();

    // 初期状態を設定（例: IdleState）
    auto ctx = PlayerStateContext{ *this };
    pStateMachine_->ChangeState(states_[static_cast<size_t>(State::Idle)].get(), ctx);
}

void Player::InitializeSkills()
{
    // オーバードライブの初期化
    pOverdrive_ = std::make_unique<Overdrive>(&comboBuffSystem_);
    pUpTempo_ = std::make_unique<UpTempo>(beatClock_);
}

void Player::InitializeObject3d()
{
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
    trueModel->SetAnimationLoop(Global::AnimationNames::Player::kAttackHorizontal, false);
    trueModel->SetAnimationSpeed(kAnimationSpeed_);
    // 攻撃用のメッシュは最初は非表示にしておく
    trueModel->SetMeshVisible("mesh_stick", false);
}

void Player::InitializeCollider()
{
    colliderTransform_ = transform_;
    colliderTransform_.translate.y = kColliderSize_->y * 0.5f;

    // コライダーの初期化
    PlayerCollider::InitData colliderInitData
    {
        .pushBackCallback = [this](const Tako::Vector3& pushBack)
    {
        transform_.translate += pushBack;
        if (pushBack.y > 0)
        {
            pMovement_->ResetVelocityY();
            pMovement_->SetGrounded(true);
        }
        pModel_->SetTransform(transform_);
    },
        .parrySuccessCallback = [this]()
    {
        // パリィ成功時の処理（例: コンボバフの付与、エフェクトの再生など）
        pParryPresentation_->Play(transform_.translate);
    },
        .comboBuffSystem = comboBuffSystem_,
        .hpComponent = *pHPComponent_,
        .parryJudgement = *pParryJudgement_,
    };

    pCollider_ = std::make_unique<PlayerCollider>(colliderInitData);
    pCollider_->SetSize(kColliderSize_);
    pCollider_->SetTransform(&colliderTransform_);
    pCollider_->SetTypeID(static_cast<uint32_t>(ColliderTypeID::Player));

    auto colManeger = Tako::CollisionManager::GetInstance();
    colManeger->AddCollider(pCollider_.get());
    colManeger->SetCollisionMask(static_cast<uint32_t>(ColliderTypeID::Player), static_cast<uint32_t>(ColliderTypeID::Terrain), true);
    colManeger->SetCollisionMask(static_cast<uint32_t>(ColliderTypeID::Player), static_cast<uint32_t>(ColliderTypeID::StageTransitionEvent), true);
    colManeger->SetCollisionMask(static_cast<uint32_t>(ColliderTypeID::Player), static_cast<uint32_t>(ColliderTypeID::Enemy), true);
    colManeger->SetCollisionMask(static_cast<uint32_t>(ColliderTypeID::PlayerAttack), static_cast<uint32_t>(ColliderTypeID::Enemy), true);
}

#pragma endregion

void Player::UpdateSkills(const PlayerInput::PlayerCommand& inputCommand)
{
    if (inputCommand.isParryTriggered)
    {
        pParryHistory_->Record();
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
    pParryPresentation_->Update();

}
