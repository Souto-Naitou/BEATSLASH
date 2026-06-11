#include "BTRetreatAction.h"
#include <BTBlackboard.h>
#include <character/boss/Boss.h>
#include <character/boss/bt/BossBlackboardKeys.h>
#include <algorithm>
#include <cmath>

#ifdef _DEBUG
#include <imgui.h>
#endif

BTRetreatAction::BTRetreatAction()
{
    name_ = "Retreat";
}

Tako::BTNodeStatus BTRetreatAction::Execute(Tako::BTBlackboard* blackboard)
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

    if (distance >= retreatDistance_)
    {
        ResetStartGate();
        status_ = Tako::BTNodeStatus::Success;
        return status_;
    }

    // ターゲットの逆方向へ離れる（重なっていて方向が取れない場合はボスの背面方向へ）
    Tako::Vector3 direction;
    if (distance > kMinDirectionDistance)
    {
        direction = toTarget.Normalize() * -1.0f;
    }
    else
    {
        float yaw = transform.rotate.y;
        direction = { -std::sin(yaw), 0.0f, -std::cos(yaw) };
    }

    float step = moveSpeed_ * blackboard->GetDeltaTime();

    bool finished = false;

    // 目標距離に届くフレームは行き過ぎないようクランプする
    float remainToGoal = retreatDistance_ - distance;
    if (step >= remainToGoal)
    {
        step = remainToGoal;
        finished = true;
    }

    // 累計移動距離が上限に達したら目標距離未達でも終了する（実行中の上限変更による負値はクランプ）
    float remainBudget = (std::max)(moveDistance_ - movedDistance_, 0.0f);
    if (step >= remainBudget)
    {
        step = remainBudget;
        finished = true;
    }

    transform.translate += direction * step;
    movedDistance_ += step;
    // 進行方向（離脱方向）へ向きを合わせる
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

void BTRetreatAction::OnStart(Tako::BTBlackboard* blackboard)
{
    (void)blackboard;
    movedDistance_ = 0.0f;
}

void BTRetreatAction::Reset()
{
    BossBTActionBase::Reset();
    movedDistance_ = 0.0f;
}

void BTRetreatAction::ApplyParameters(const nlohmann::json& params)
{
    BossBTActionBase::ApplyParameters(params);
    if (params.contains("retreatDistance"))
    {
        retreatDistance_ = params["retreatDistance"].get<float>();
    }
    if (params.contains("moveSpeed"))
    {
        moveSpeed_ = params["moveSpeed"].get<float>();
    }
    if (params.contains("moveDistance"))
    {
        moveDistance_ = params["moveDistance"].get<float>();
    }
}

nlohmann::json BTRetreatAction::ExtractParameters() const
{
    nlohmann::json params = BossBTActionBase::ExtractParameters();
    params["retreatDistance"] = retreatDistance_;
    params["moveSpeed"] = moveSpeed_;
    params["moveDistance"] = moveDistance_;
    return params;
}

#ifdef _DEBUG
bool BTRetreatAction::DrawImGui()
{
    bool changed = BossBTActionBase::DrawImGui();
    changed |= ImGui::DragFloat("Retreat Distance", &retreatDistance_, 0.1f, 0.1f, 100.0f);
    changed |= ImGui::DragFloat("Move Speed", &moveSpeed_, 0.1f, 0.1f, 50.0f);
    changed |= ImGui::DragFloat("Move Distance", &moveDistance_, 0.1f, 0.1f, 100.0f);
    return changed;
}
#endif
