#include "Boss.h"
#include <CollisionManager.h>
#include <EmitterManager.h>
#include <manager/BeatManager.h>
#include <type/ColliderTypeID.h>
#include <character/boss/bt/BossBlackboardKeys.h>
#include <common/ParticleEmitterPresetNames.h>
#include <utility/DeltaTimeManager.h>
#include <numbers>

#ifdef _DEBUG
#include <imgui.h>
#include <DebugUIManager.h>
#endif

Boss::Boss(const ICharacter* target, const BeatClock* beatClock, Tako::EmitterManager* emitterManager)
    : pTarget_(target)
    , pBeatClock_(beatClock)
    , pEmitterManager_(emitterManager)
{}

Boss::~Boss()
{
    // コライダーをマネージャーから削除
    Tako::CollisionManager::GetInstance()->RemoveCollider(pCollider_.get());

#ifdef _DEBUG
    Tako::DebugUIManager::GetInstance()->UnregisterGameObject("Boss");
#endif
}

void Boss::Initialize()
{
    // モデルの初期化
    pModel_ = std::make_unique<Tako::Object3d>();
    pModel_->Initialize();
    pModel_->SetModel("white_cube.gltf");
    pModel_->SetMaterialColor(kInitialMaterialColor);
    pModel_->SetEnableLighting(true);
    pModel_->SetTranslate(kInitialTranslate);
    pModel_->SetScale(kInitialScale);

    // トランスフォームの初期化
    transform_ = pModel_->GetTransform();

    // 移動コンポーネントの初期化
    pMovement_ = std::make_unique<PhysicsMovement>();

    // コライダーの初期化
    pCollider_ = std::make_unique<BossCollider>();
    pCollider_->SetSize(pModel_->GetScale() * kColliderScaleMultiplier);
    pCollider_->SetOwner(this);
    pCollider_->SetTypeID(static_cast<uint32_t>(ColliderTypeID::Boss));
    pCollider_->SetTransform(&transform_);
    pCollider_->SetPushBackCallback([this](const Tako::Vector3& pushBack) {
        transform_.translate += pushBack;
        // 接地時は落下速度をリセットする
        if (pushBack.y > 0)
        {
            pMovement_->ResetVelocityY();
        }
        pModel_->SetTransform(transform_);
        pModel_->Update();
                                    });

    // コライダーをマネージャーに登録
    auto collisionManager = Tako::CollisionManager::GetInstance();
    collisionManager->AddCollider(pCollider_.get());
    collisionManager->SetCollisionMask(static_cast<uint32_t>(ColliderTypeID::Boss), static_cast<uint32_t>(ColliderTypeID::Player), true);
    collisionManager->SetCollisionMask(static_cast<uint32_t>(ColliderTypeID::Boss), static_cast<uint32_t>(ColliderTypeID::PlayerAttack), true);
    collisionManager->SetCollisionMask(static_cast<uint32_t>(ColliderTypeID::Boss), static_cast<uint32_t>(ColliderTypeID::Enemy), true);
    collisionManager->SetCollisionMask(static_cast<uint32_t>(ColliderTypeID::Boss), static_cast<uint32_t>(ColliderTypeID::Terrain), true);

    // HPコンポーネントの生成と初期化
    pHp_ = std::make_unique<HPComponent>();
    pHp_->Initialize(kInitialHP);
    pCollider_->SetHPComponent(pHp_.get());

    // レーザー攻撃用エミッタープリセットのロード（テンプレートとして保持するため非活性化）
    // ボス再生成時の二重登録（旧エミッタの残留）を防ぐためロード済みならスキップする
    if (pEmitterManager_)
    {
        using namespace Global::ParticleEmitterPresetNames;
        for (const char* preset : { kBossRazerBall, kBossRazerCharge, kBossRazerTrail })
        {
            if (!pEmitterManager_->HasEmitter(preset))
            {
                pEmitterManager_->LoadPreset(preset);
            }
            pEmitterManager_->SetEmitterActive(preset, false);
        }
    }

    // ビヘイビアツリーの構築（JSONロード前にブラックボードを設定しておく）
    pBehaviorTree_ = std::make_unique<Tako::BehaviorTree>();
    SetupBlackboard();
    if (!pBehaviorTree_->LoadFromJSON(kBehaviorTreeJsonPath))
    {
#ifdef _DEBUG
        Tako::DebugUIManager::GetInstance()->AddLog(
            std::string("Boss: ビヘイビアツリーの読み込みに失敗: ") + kBehaviorTreeJsonPath,
            Tako::DebugUIManager::LogType::Error);
#endif
    }

}

void Boss::Update()
{
    const float deltaTime = DeltaTimeManager::GetInstance()->GetDeltaTime(DeltaTimeChannelReserved::Game);

    // 拍境界の検知結果と拍情報をブラックボードへ共有する
    beatEdge_.Update(pBeatClock_, pBehaviorTree_->GetBlackboard());

    // 生存中のみビヘイビアツリーを実行する
    if (IsAlive())
    {
        pBehaviorTree_->Tick(deltaTime);
    }

    // 拍同期の拡縮アニメーションを適用する
    UpdateBeatAnimation();

    // 重力を適用し、速度と加速度に基づいて位置を更新する
    pMovement_->ApplyGravity(kMass, deltaTime);
    pMovement_->Update(transform_, deltaTime);

    // トランスフォームの更新
    pModel_->SetTransform(transform_);
    pModel_->Update();
}

void Boss::Draw()
{
    pModel_->Draw();
}

void Boss::SetBehaviorTreeRoot(Tako::BTNodePtr root)
{
    // 旧ツリーのノードをResetし、実行中ノードのエフェクトやコライダーを掃除してから差し替える
    pBehaviorTree_->Reset();
    pBehaviorTree_->SetRootNode(root);
    pBehaviorTree_->Reset();
}

void Boss::SetupBlackboard()
{
    auto* blackboard = pBehaviorTree_->GetBlackboard();
    blackboard->SetPtr<Boss>(BossBlackboardKeys::kBoss, this);
    blackboard->SetPtr<const ICharacter>(BossBlackboardKeys::kTarget, pTarget_);
    blackboard->SetPtr<const BeatClock>(BossBlackboardKeys::kBeatClock, pBeatClock_);
    blackboard->SetPtr<Tako::EmitterManager>(BossBlackboardKeys::kEmitterManager, pEmitterManager_);
}

void Boss::DrawImGui()
{
#ifdef _DEBUG
    ImGui::SeparatorText("Transform");
    ImGui::SliderFloat3("Position", &transform_.translate.x, -50.0f, 50.0f);
    ImGui::SliderFloat3("Rotation", &transform_.rotate.x, -3.14f, 3.14f);
    ImGui::SliderFloat3("Scale", &transform_.scale.x, 0.1f, 5.0f);

    ImGui::SeparatorText("Status");
    if (pHp_)
    {
        ImGui::Text("HP: %d / %d", pHp_->GetCurrentHP(), kInitialHP);
    }
    ImGui::Text("Beat Passed: %s", beatEdge_.IsBeatPassed() ? "true" : "false");

    ImGui::SeparatorText("Behavior Tree");
    auto runningNode = pBehaviorTree_ ? pBehaviorTree_->GetCurrentRunningNode() : nullptr;
    ImGui::Text("Running Node: %s", runningNode ? runningNode->GetName().c_str() : "(none)");

    if (nodeEditorToggleCallback_ && ImGui::Button("Node Editor"))
    {
        nodeEditorToggleCallback_();
    }
#endif
}

void Boss::UpdateBeatAnimation()
{
    if (pBeatClock_)
    {
        // 拍に同期したコサイン波で拡縮する
        float beat = pBeatClock_->GetCurrentBeat();
        float scale = baseScale_ + scaleAmplitude_ * std::cos(beat * 2.0f * std::numbers::pi_v<float>);
        SetScale({ scale, scale, scale });
    }
}
