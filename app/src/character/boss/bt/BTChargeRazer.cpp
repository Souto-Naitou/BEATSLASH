#include "BTChargeRazer.h"
#include <BTBlackboard.h>
#include <CollisionManager.h>
#include <EmitterManager.h>
#include <SphereEmitter.h>
#include <BoxEmitter.h>
#include <character/boss/Boss.h>
#include <character/boss/collider/BossRazerCollider.h>
#include <character/boss/bt/BossBlackboardKeys.h>
#include <common/ParticleEmitterPresetNames.h>
#include <manager/BeatManager.h>
#include <type/ColliderTypeID.h>
#include <algorithm>
#include <cmath>
#include <numbers>

#ifdef _DEBUG
#include <imgui.h>
#endif

namespace
{
    namespace Presets = Global::ParticleEmitterPresetNames;
}

uint32_t BTChargeRazer::sTrailCounter_ = 0;

BTChargeRazer::BTChargeRazer()
{
    name_ = "ChargeRazer";
}

BTChargeRazer::~BTChargeRazer()
{
    RemoveRazerCollider();
}

Tako::BTNodeStatus BTChargeRazer::Execute(Tako::BTBlackboard* blackboard)
{
    Boss* boss = blackboard->GetPtr<Boss>(BossBlackboardKeys::kBoss);
    Tako::EmitterManager* emitterManager = blackboard->GetPtr<Tako::EmitterManager>(BossBlackboardKeys::kEmitterManager);
    if (!boss || !emitterManager)
    {
        status_ = Tako::BTNodeStatus::Failure;
        return status_;
    }
    pEmitterManager_ = emitterManager;

    const BeatClock* beatClock = blackboard->GetPtr<const BeatClock>(BossBlackboardKeys::kBeatClock);
    const ICharacter* target = blackboard->GetPtr<const ICharacter>(BossBlackboardKeys::kTarget);

    Tako::Vector3 ballPos = boss->GetPosition() + ballOffset_;

    if (!isStarted_)
    {
        StartCharging(ballPos, beatClock);
    }

    if (phase_ == Phase::Charging)
    {
        // チャージエフェクトをボス頭上へ追従させ、収束先も同位置に保つ
        pEmitterManager_->SetEmitterPosition(Presets::kBossRazerBall, ballPos);
        pEmitterManager_->SetEmitterPosition(Presets::kBossRazerCharge, ballPos);
        if (auto charge = pEmitterManager_->GetEmitterByName(Presets::kBossRazerCharge))
        {
            charge->SetTargetPosition(ballPos);
        }

        // 経過拍の算出（BeatClock不在時は経過秒を拍とみなす）
        float elapsedBeats = 0.0f;
        if (beatClock)
        {
            elapsedBeats = beatClock->GetCurrentBeat() - startBeat_;
        }
        else
        {
            elapsedSeconds_ += blackboard->GetDeltaTime();
            elapsedBeats = elapsedSeconds_;
        }

        // 間隔を跨いだ分だけチャージする
        while (performedCharges_ < chargeCount_ &&
               elapsedBeats >= static_cast<float>(performedCharges_ + 1) * chargeIntervalBeats_)
        {
            ApplyChargeStep();
        }

        if (performedCharges_ >= chargeCount_)
        {
            // ターゲット不在時はボス正面へ発射する
            Tako::Vector3 targetPos;
            if (target)
            {
                targetPos = target->GetPosition() + Tako::Vector3{ 0.0f, aimOffsetY_, 0.0f };
            }
            else
            {
                float bossYaw = boss->GetRotation().y;
                targetPos = ballPos + Tako::Vector3{ std::sin(bossYaw), 0.0f, std::cos(bossYaw) } * 10.0f;
            }

            Fire(ballPos, targetPos);
            DeactivateChargeEffects();
            phase_ = Phase::Firing;
            colliderTimer_.Enable(fireDuration_);
        }

        status_ = Tako::BTNodeStatus::Running;
        return status_;
    }

    // 発射フェーズ: コライダーの有効期間が切れたら完了
    colliderTimer_.Update(blackboard->GetDeltaTime());
    if (!colliderTimer_.IsActive())
    {
        RemoveRazerCollider();
        isStarted_ = false;
        phase_ = Phase::Charging;
        status_ = Tako::BTNodeStatus::Success;
        return status_;
    }

    status_ = Tako::BTNodeStatus::Running;
    return status_;
}

void BTChargeRazer::StartCharging(const Tako::Vector3& ballPos, const BeatClock* beatClock)
{
    isStarted_ = true;
    phase_ = Phase::Charging;
    performedCharges_ = 0;
    elapsedSeconds_ = 0.0f;
    startBeat_ = beatClock ? beatClock->GetCurrentBeat() : 0.0f;

    // ball: 半径を開始値へ戻して表示
    auto ball = pEmitterManager_->GetEmitterByName(Presets::kBossRazerBall);
    if (auto sphere = std::dynamic_pointer_cast<Tako::SphereEmitter>(ball))
    {
        sphere->SetRadius(ballStartRadius_);
    }
    pEmitterManager_->SetEmitterPosition(Presets::kBossRazerBall, ballPos);
    if (ball)
    {
        ball->SetActive(true);
        ball->SetEmitting(true);
    }

    // charge: ballと同位置に置き、収束先もballの位置にする
    pEmitterManager_->SetEmitterPosition(Presets::kBossRazerCharge, ballPos);
    if (auto charge = pEmitterManager_->GetEmitterByName(Presets::kBossRazerCharge))
    {
        charge->SetTargetPosition(ballPos);
        charge->SetActive(true);
        charge->SetEmitting(true);
    }
}

void BTChargeRazer::ApplyChargeStep()
{
    if (auto sphere = std::dynamic_pointer_cast<Tako::SphereEmitter>(
            pEmitterManager_->GetEmitterByName(Presets::kBossRazerBall)))
    {
        sphere->SetRadius(sphere->GetRadius() + radiusPerCharge_);
    }
    ++performedCharges_;
}

void BTChargeRazer::Fire(const Tako::Vector3& ballPos, const Tako::Vector3& targetPos)
{
    Tako::Vector3 toTarget = targetPos - ballPos;
    float distance = (std::max)(toTarget.Length(), kMinFireDistance);
    Tako::Vector3 dir = toTarget * (1.0f / distance);

    float pitch = -std::asin(std::clamp(dir.y, -1.0f, 1.0f));
    float yaw = std::atan2(dir.x, dir.z);
    // 追加長の分だけ先端をターゲットの先へ伸ばす
    float length = distance + laserExtraLength_;
    Tako::Vector3 mid = ballPos + dir * (length * 0.5f);
    constexpr float kRadToDeg = 180.0f / std::numbers::pi_v<float>;

    // レーザー本体: 一時エミッターを作成し、エミッターボックス（中心±size/2）をball→ターゲット間に合わせる
    const std::string trailName = std::string(Presets::kBossRazerTrail) + "_" + std::to_string(sTrailCounter_++);
    pEmitterManager_->CreateTemporaryEmitterFrom(Presets::kBossRazerTrail, trailName, fireDuration_);

    Tako::Vector3 boxSize = { 0.3f * laserThicknessScale_, 0.3f * laserThicknessScale_, length };
    if (auto box = std::dynamic_pointer_cast<Tako::BoxEmitter>(pEmitterManager_->GetEmitterByName(trailName)))
    {
        // 断面はプリセット値×倍率、長さは距離+追加長に合わせる
        boxSize = { box->GetSize().x * laserThicknessScale_, box->GetSize().y * laserThicknessScale_, length };
        box->SetSize(boxSize);
		// エミッターの回転を、ball→ターゲット方向に合わせる（度数法）
        box->SetRotation({ pitch * kRadToDeg, yaw * kRadToDeg, 0.0f });
    }
    pEmitterManager_->SetEmitterPosition(trailName, mid);
    pEmitterManager_->SetEmitterActive(trailName, true);

    // 当たり判定: エミッターと同じ位置・サイズ・角度のOBB（ラジアン）
    colliderTransform_.translate = mid;
    colliderTransform_.rotate = { pitch, yaw, 0.0f };
    colliderTransform_.scale = { 1.0f, 1.0f, 1.0f };

    if (!pRazerCollider_)
    {
        pRazerCollider_ = std::make_unique<BossRazerCollider>();
    }
    pRazerCollider_->SetSize(boxSize);
    pRazerCollider_->SetTransform(&colliderTransform_);
    pRazerCollider_->SetTypeID(static_cast<uint32_t>(ColliderTypeID::BossAttack));
    pRazerCollider_->SetOwner(this);
    pRazerCollider_->SetActive(true);

    auto collisionManager = Tako::CollisionManager::GetInstance();
    collisionManager->AddCollider(pRazerCollider_.get());
    collisionManager->SetCollisionMask(static_cast<uint32_t>(ColliderTypeID::BossAttack), static_cast<uint32_t>(ColliderTypeID::Player), true);
    colliderRegistered_ = true;
}

void BTChargeRazer::DeactivateChargeEffects()
{
    if (!pEmitterManager_)
    {
        return;
    }
    if (auto ball = pEmitterManager_->GetEmitterByName(Presets::kBossRazerBall))
    {
        ball->SetEmitting(false);
        ball->SetActive(false);
    }
    if (auto charge = pEmitterManager_->GetEmitterByName(Presets::kBossRazerCharge))
    {
        charge->SetEmitting(false);
        charge->SetActive(false);
    }
}

void BTChargeRazer::RemoveRazerCollider()
{
    if (colliderRegistered_ && pRazerCollider_)
    {
        Tako::CollisionManager::GetInstance()->RemoveCollider(pRazerCollider_.get());
        colliderRegistered_ = false;
    }
}

void BTChargeRazer::Reset()
{
    BTNode::Reset();
    DeactivateChargeEffects();
    RemoveRazerCollider();
    colliderTimer_ = {};
    isStarted_ = false;
    phase_ = Phase::Charging;
    performedCharges_ = 0;
    elapsedSeconds_ = 0.0f;
}

void BTChargeRazer::ApplyParameters(const nlohmann::json& params)
{
    if (params.contains("chargeIntervalBeats"))
    {
        chargeIntervalBeats_ = params["chargeIntervalBeats"].get<float>();
    }
    if (params.contains("chargeCount"))
    {
        chargeCount_ = params["chargeCount"].get<int>();
    }
    if (params.contains("radiusPerCharge"))
    {
        radiusPerCharge_ = params["radiusPerCharge"].get<float>();
    }
    if (params.contains("ballOffset") && params["ballOffset"].is_array() && params["ballOffset"].size() >= 3)
    {
        ballOffset_ = {
            params["ballOffset"][0].get<float>(),
            params["ballOffset"][1].get<float>(),
            params["ballOffset"][2].get<float>(),
        };
    }
    if (params.contains("ballStartRadius"))
    {
        ballStartRadius_ = params["ballStartRadius"].get<float>();
    }
    if (params.contains("fireDuration"))
    {
        fireDuration_ = params["fireDuration"].get<float>();
    }
    if (params.contains("laserThicknessScale"))
    {
        laserThicknessScale_ = params["laserThicknessScale"].get<float>();
    }
    if (params.contains("laserExtraLength"))
    {
        laserExtraLength_ = params["laserExtraLength"].get<float>();
    }
    if (params.contains("aimOffsetY"))
    {
        aimOffsetY_ = params["aimOffsetY"].get<float>();
    }
}

nlohmann::json BTChargeRazer::ExtractParameters() const
{
    return nlohmann::json{
        { "chargeIntervalBeats", chargeIntervalBeats_ },
        { "chargeCount", chargeCount_ },
        { "radiusPerCharge", radiusPerCharge_ },
        { "ballOffset", { ballOffset_.x, ballOffset_.y, ballOffset_.z } },
        { "ballStartRadius", ballStartRadius_ },
        { "fireDuration", fireDuration_ },
        { "laserThicknessScale", laserThicknessScale_ },
        { "laserExtraLength", laserExtraLength_ },
        { "aimOffsetY", aimOffsetY_ },
    };
}

#ifdef _DEBUG
bool BTChargeRazer::DrawImGui()
{
    bool changed = false;
    ImGui::SeparatorText("Charge");
    changed |= ImGui::DragFloat("Charge Interval (Beats)", &chargeIntervalBeats_, 0.1f, 0.25f, 16.0f);
    changed |= ImGui::DragInt("Charge Count", &chargeCount_, 1, 1, 10);
    changed |= ImGui::DragFloat("Radius Per Charge", &radiusPerCharge_, 0.05f, 0.0f, 5.0f);
    changed |= ImGui::DragFloat3("Ball Offset", &ballOffset_.x, 0.1f, -20.0f, 20.0f);
    changed |= ImGui::DragFloat("Ball Start Radius", &ballStartRadius_, 0.05f, 0.0f, 10.0f);
    ImGui::SeparatorText("Fire");
    changed |= ImGui::DragFloat("Fire Duration", &fireDuration_, 0.01f, 0.01f, 2.0f);
    changed |= ImGui::DragFloat("Laser Thickness Scale", &laserThicknessScale_, 0.05f, 0.1f, 10.0f);
    changed |= ImGui::DragFloat("Laser Extra Length", &laserExtraLength_, 0.1f, 0.0f, 50.0f);
    changed |= ImGui::DragFloat("Aim Offset Y", &aimOffsetY_, 0.1f, -10.0f, 10.0f);
    return changed;
}
#endif
