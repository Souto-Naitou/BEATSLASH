#include "BTMeleeAttackAction.h"
#include <BTBlackboard.h>
#include <CollisionManager.h>
#include <EmitterManager.h>
#include <character/boss/Boss.h>
#include <character/boss/bt/BossBlackboardKeys.h>
#include <manager/BeatManager.h>
#include <type/ColliderTypeID.h>
#include <algorithm>
#include <cmath>
#include <numbers>

#ifdef _DEBUG
#include <imgui.h>
#endif

uint32_t BTMeleeAttackAction::sInstanceCounter_ = 0;

BTMeleeAttackAction::BTMeleeAttackAction()
{
    name_ = "MeleeAttack";
}

BTMeleeAttackAction::~BTMeleeAttackAction()
{
    RemoveBarCollider();

    // 棒モデルにバインドしたエフェクトを削除する（残すと破棄済みモデルを参照するエミッターが蓄積する）
    if (pEmitterManager_)
    {
        if (!effectBaseName_.empty())
        {
            pEmitterManager_->RemoveEmitter(effectBaseName_);
        }
        if (!effectTempName_.empty())
        {
            pEmitterManager_->RemoveEmitter(effectTempName_);
        }
    }
}

Tako::BTNodeStatus BTMeleeAttackAction::Execute(Tako::BTBlackboard* blackboard)
{
    Boss* boss = blackboard->GetPtr<Boss>(BossBlackboardKeys::kBoss);
    const ICharacter* target = blackboard->GetPtr<const ICharacter>(BossBlackboardKeys::kTarget);
    if (!boss || !target)
    {
        status_ = Tako::BTNodeStatus::Failure;
        return status_;
    }
    pEmitterManager_ = blackboard->GetPtr<Tako::EmitterManager>(BossBlackboardKeys::kEmitterManager);

    // 拍境界まで開始を待つ
    if (!UpdateStartGate(blackboard))
    {
        status_ = Tako::BTNodeStatus::Running;
        return status_;
    }

    const BeatClock* beatClock = blackboard->GetPtr<const BeatClock>(BossBlackboardKeys::kBeatClock);
    Tako::Transform& transform = boss->GetTransform();

    // 経過拍の算出（BeatClock不在時は経過秒を拍とみなす）
    float elapsedBeats = 0.0f;
    if (beatClock)
    {
        elapsedBeats = beatClock->GetCurrentBeat() - GetStartBeat();
    }
    else
    {
        elapsedSeconds_ += blackboard->GetDeltaTime();
        elapsedBeats = elapsedSeconds_;
    }

    const float halfAngle = swingHalfAngleDeg_ * std::numbers::pi_v<float> / 180.0f;

    if (phase_ == Phase::Growing)
    {
        // 成長中はボスをターゲットの方向へ向け続ける
        Tako::Vector3 toTarget = target->GetPosition() - transform.translate;
        toTarget.y = 0.0f;
        if (toTarget.Length() > 0.001f)
        {
            transform.rotate.y = std::atan2(toTarget.x, toTarget.z);
        }

        // 間隔を跨いだ分だけ棒を伸ばす
        while (performedGrows_ < growCount_ &&
               elapsedBeats >= static_cast<float>(performedGrows_ + 1) * growIntervalBeats_)
        {
            currentLength_ += lengthPerGrow_;
            ++performedGrows_;
        }

        // 振り開始位置（正面から+半角）の円周上に棒を追従させる
        UpdateBarPlacement(transform.translate, transform.rotate.y + halfAngle);

        // 予告の赤点滅
        float alpha = 120.0f + 80.0f * std::sin(elapsedBeats * 10.0f);
        pBarModel_->SetMaterialColor({ 256.0f, 0.0f, 0.0f, alpha });
        pBarModel_->SetTransparent(true);

        if (performedGrows_ >= growCount_)
        {
            StartSwing(transform.rotate.y, beatClock);
        }
    }
    else
    {
        float swingStartBeats = static_cast<float>(growCount_) * growIntervalBeats_;
        float progress = std::clamp((elapsedBeats - swingStartBeats) / swingDurationBeats_, 0.0f, 1.0f);

        // easeInOutQuintで+半角から-半角まで振り抜く
        float easedT = progress < 0.5f
            ? 16.0f * progress * progress * progress * progress * progress
            : 1.0f - std::pow(-2.0f * progress + 2.0f, 5.0f) / 2.0f;
        float currentAngle = swingBaseYaw_ + halfAngle * (1.0f - 2.0f * easedT);

        // 振り中はボスの向きを固定する
        transform.rotate.y = swingBaseYaw_;

        UpdateBarPlacement(transform.translate, currentAngle);

        if (progress >= 1.0f)
        {
            RemoveBarCollider();
            ResetStartGate();
            phase_ = Phase::Growing;
            status_ = Tako::BTNodeStatus::Success;
            return status_;
        }
    }

    // モデルの更新と今フレームの描画登録
    pBarModel_->SetTransform(barTransform_);
    pBarModel_->Update();
    boss->QueueAttachedModelDraw(pBarModel_.get());

    status_ = Tako::BTNodeStatus::Running;
    return status_;
}

void BTMeleeAttackAction::OnStart(Tako::BTBlackboard* blackboard)
{
    (void)blackboard;
    phase_ = Phase::Growing;
    performedGrows_ = 0;
    currentLength_ = barStartLength_;
    elapsedSeconds_ = 0.0f;

    // 初回のみ棒モデルとエフェクトテンプレートを生成する
    if (!pBarModel_)
    {
        pBarModel_ = std::make_unique<Tako::Object3d>();
        pBarModel_->Initialize();
        pBarModel_->SetModel("white_cube.gltf");
        pBarModel_->SetEnableLighting(true);

        if (pEmitterManager_)
        {
            effectBaseName_ = std::string(kEffectPresetName) + "_boss_" + std::to_string(sInstanceCounter_++);
            pEmitterManager_->LoadPreset(kEffectPresetName, effectBaseName_, pBarModel_.get());
            pEmitterManager_->SetEmitterActive(effectBaseName_, false);
        }
    }
}

void BTMeleeAttackAction::UpdateBarPlacement(const Tako::Vector3& bossPos, float angle)
{
    Tako::Vector3 offset = { std::sin(angle) * offsetDistance_, 0.0f, std::cos(angle) * offsetDistance_ };
    barTransform_.translate = bossPos + offset;
    barTransform_.translate.y += barOffsetY_;
    // 円周の接線方向へ向ける
    barTransform_.rotate = { 0.0f, angle + std::numbers::pi_v<float> / 2.0f, 0.0f };
    barTransform_.scale = { currentLength_, barHeight_, barDepth_ };
}

void BTMeleeAttackAction::StartSwing(float baseYaw, const BeatClock* beatClock)
{
    phase_ = Phase::Swinging;
    swingBaseYaw_ = baseYaw;

    // 振りの間だけ有効な当たり判定を登録する
    if (!pBarCollider_)
    {
        pBarCollider_ = std::make_unique<Tako::OBBCollider>();
    }
    pBarCollider_->SetSize(Tako::Vector3{ currentLength_, barHeight_, barDepth_ } * kColliderSizeMultiplier);
    pBarCollider_->SetOwner(this);
    pBarCollider_->SetTypeID(static_cast<uint32_t>(ColliderTypeID::BossAttack));
    pBarCollider_->SetTransform(&barTransform_);
    pBarCollider_->SetActive(true);

    auto collisionManager = Tako::CollisionManager::GetInstance();
    collisionManager->AddCollider(pBarCollider_.get());
    collisionManager->SetCollisionMask(static_cast<uint32_t>(ColliderTypeID::BossAttack), static_cast<uint32_t>(ColliderTypeID::Player), true);
    colliderRegistered_ = true;

    // 攻撃中の色（オレンジ）へ切り替える
    pBarModel_->SetMaterialColor({ 256.0f, 128.0f, 0.0f, 256.0f });
    pBarModel_->SetTransparent(false);

    // 攻撃エフェクトを棒に沿って再生する（振り時間と同じ寿命の一時エミッター）
    if (pEmitterManager_ && !effectBaseName_.empty())
    {
        float secondsPerBeat = beatClock ? beatClock->GetSecondsPerBeat() : 1.0f;
        effectTempName_ = effectBaseName_ + "_temp_" + std::to_string(playCount_++);
        pEmitterManager_->CreateTemporaryEmitterFrom(effectBaseName_, effectTempName_, swingDurationBeats_ * secondsPerBeat);
    }
}

void BTMeleeAttackAction::RemoveBarCollider()
{
    if (colliderRegistered_ && pBarCollider_)
    {
        Tako::CollisionManager::GetInstance()->RemoveCollider(pBarCollider_.get());
        colliderRegistered_ = false;
    }
}

void BTMeleeAttackAction::Reset()
{
    BossBTActionBase::Reset();
    RemoveBarCollider();
    phase_ = Phase::Growing;
    performedGrows_ = 0;
    elapsedSeconds_ = 0.0f;
}

void BTMeleeAttackAction::ApplyParameters(const nlohmann::json& params)
{
    BossBTActionBase::ApplyParameters(params);
    if (params.contains("growIntervalBeats"))
    {
        growIntervalBeats_ = params["growIntervalBeats"].get<float>();
    }
    if (params.contains("growCount"))
    {
        growCount_ = params["growCount"].get<int>();
    }
    if (params.contains("swingDurationBeats"))
    {
        swingDurationBeats_ = params["swingDurationBeats"].get<float>();
    }
    if (params.contains("barStartLength"))
    {
        barStartLength_ = params["barStartLength"].get<float>();
    }
    if (params.contains("lengthPerGrow"))
    {
        lengthPerGrow_ = params["lengthPerGrow"].get<float>();
    }
    if (params.contains("barHeight"))
    {
        barHeight_ = params["barHeight"].get<float>();
    }
    if (params.contains("barDepth"))
    {
        barDepth_ = params["barDepth"].get<float>();
    }
    if (params.contains("offsetDistance"))
    {
        offsetDistance_ = params["offsetDistance"].get<float>();
    }
    if (params.contains("barOffsetY"))
    {
        barOffsetY_ = params["barOffsetY"].get<float>();
    }
    if (params.contains("swingHalfAngleDeg"))
    {
        swingHalfAngleDeg_ = params["swingHalfAngleDeg"].get<float>();
    }
}

nlohmann::json BTMeleeAttackAction::ExtractParameters() const
{
    nlohmann::json params = BossBTActionBase::ExtractParameters();
    params["growIntervalBeats"] = growIntervalBeats_;
    params["growCount"] = growCount_;
    params["swingDurationBeats"] = swingDurationBeats_;
    params["barStartLength"] = barStartLength_;
    params["lengthPerGrow"] = lengthPerGrow_;
    params["barHeight"] = barHeight_;
    params["barDepth"] = barDepth_;
    params["offsetDistance"] = offsetDistance_;
    params["barOffsetY"] = barOffsetY_;
    params["swingHalfAngleDeg"] = swingHalfAngleDeg_;
    return params;
}

#ifdef _DEBUG
bool BTMeleeAttackAction::DrawImGui()
{
    bool changed = BossBTActionBase::DrawImGui();
    ImGui::SeparatorText("Grow");
    changed |= ImGui::DragFloat("Grow Interval (Beats)", &growIntervalBeats_, 0.1f, 0.25f, 16.0f);
    changed |= ImGui::DragInt("Grow Count", &growCount_, 1, 1, 10);
    changed |= ImGui::DragFloat("Bar Start Length", &barStartLength_, 0.1f, 0.5f, 30.0f);
    changed |= ImGui::DragFloat("Length Per Grow", &lengthPerGrow_, 0.1f, 0.0f, 10.0f);
    ImGui::SeparatorText("Bar");
    changed |= ImGui::DragFloat("Bar Height", &barHeight_, 0.05f, 0.05f, 5.0f);
    changed |= ImGui::DragFloat("Bar Depth", &barDepth_, 0.05f, 0.05f, 5.0f);
    changed |= ImGui::DragFloat("Offset Distance", &offsetDistance_, 0.1f, 1.0f, 30.0f);
    changed |= ImGui::DragFloat("Bar Offset Y", &barOffsetY_, 0.1f, -20.0f, 20.0f);
    ImGui::SeparatorText("Swing");
    changed |= ImGui::DragFloat("Swing Duration (Beats)", &swingDurationBeats_, 0.1f, 0.25f, 8.0f);
    changed |= ImGui::DragFloat("Swing Half Angle (Deg)", &swingHalfAngleDeg_, 1.0f, 10.0f, 180.0f);
    return changed;
}
#endif
