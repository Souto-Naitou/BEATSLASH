#include "BTApproachAction.h"
#include <BTBlackboard.h>
#include <character/boss/Boss.h>
#include <character/boss/bt/BossBlackboardKeys.h>
#include <cmath>

#ifdef _DEBUG
#include <imgui.h>
#endif

BTApproachAction::BTApproachAction()
{
    name_ = "Approach";
}

Tako::BTNodeStatus BTApproachAction::Execute(Tako::BTBlackboard* blackboard)
{
    Boss* boss = blackboard->GetPtr<Boss>(BossBlackboardKeys::kBoss);
    const ICharacter* target = blackboard->GetPtr<const ICharacter>(BossBlackboardKeys::kTarget);
    if (!boss || !target)
    {
        status_ = Tako::BTNodeStatus::Failure;
        return status_;
    }

    // 拍境界まで開始を待つ
    if (!UpdateStartGate(blackboard))
    {
        status_ = Tako::BTNodeStatus::Running;
        return status_;
    }

    Tako::Transform& transform = boss->GetTransform();

    Tako::Vector3 toTarget = target->GetPosition() - transform.translate;
    toTarget.y = 0.0f;
    float distance = toTarget.Length();

    if (distance <= stopDistance_)
    {
        ResetStartGate();
        status_ = Tako::BTNodeStatus::Success;
        return status_;
    }

    float step = moveSpeed_ * blackboard->GetDeltaTime();
    Tako::Vector3 direction = toTarget.Normalize();

    bool finished = false;

    // 停止距離を割り込まないよう残距離でクランプする
    float remain = distance - stopDistance_;
    if (step >= remain)
    {
        step = remain;
        finished = true;
    }

    transform.translate += direction * step;
    // 進行方向（ターゲット方向）へ向きを合わせる
    transform.rotate.y = std::atan2(direction.x, direction.z);

    if (finished)
    {
        ResetStartGate();
        status_ = Tako::BTNodeStatus::Success;
        return status_;
    }

    status_ = Tako::BTNodeStatus::Running;
    return status_;
}

void BTApproachAction::ApplyParameters(const nlohmann::json& params)
{
    BossBTActionBase::ApplyParameters(params);
    if (params.contains("stopDistance"))
    {
        stopDistance_ = params["stopDistance"].get<float>();
    }
    if (params.contains("moveSpeed"))
    {
        moveSpeed_ = params["moveSpeed"].get<float>();
    }
}

nlohmann::json BTApproachAction::ExtractParameters() const
{
    nlohmann::json params = BossBTActionBase::ExtractParameters();
    params["stopDistance"] = stopDistance_;
    params["moveSpeed"] = moveSpeed_;
    return params;
}

#ifdef _DEBUG
bool BTApproachAction::DrawImGui()
{
    bool changed = BossBTActionBase::DrawImGui();
    changed |= ImGui::DragFloat("Stop Distance", &stopDistance_, 0.1f, 0.1f, 50.0f);
    changed |= ImGui::DragFloat("Move Speed", &moveSpeed_, 0.1f, 0.1f, 50.0f);
    return changed;
}
#endif
