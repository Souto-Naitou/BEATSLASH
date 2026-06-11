#include "BTWaitAction.h"
#include <BTBlackboard.h>
#include <manager/BeatManager.h>
#include <character/boss/bt/BossBlackboardKeys.h>

#ifdef _DEBUG
#include <imgui.h>
#endif

BTWaitAction::BTWaitAction()
{
    name_ = "Wait";
}

Tako::BTNodeStatus BTWaitAction::Execute(Tako::BTBlackboard* blackboard)
{
    // 拍境界まで開始を待つ
    if (!UpdateStartGate(blackboard))
    {
        status_ = Tako::BTNodeStatus::Running;
        return status_;
    }

    const BeatClock* beatClock = blackboard->GetPtr<const BeatClock>(BossBlackboardKeys::kBeatClock);

    if (beatClock)
    {
        // 開始拍は整数拍へスナップ済みのため、待機終了も拍境界に一致する
        if (beatClock->GetCurrentBeat() - GetStartBeat() >= waitBeats_)
        {
            ResetStartGate();
            status_ = Tako::BTNodeStatus::Success;
            return status_;
        }
    }
    else
    {
        elapsedSeconds_ += blackboard->GetDeltaTime();
        if (elapsedSeconds_ >= waitBeats_)
        {
            ResetStartGate();
            status_ = Tako::BTNodeStatus::Success;
            return status_;
        }
    }

    status_ = Tako::BTNodeStatus::Running;
    return status_;
}

void BTWaitAction::OnStart(Tako::BTBlackboard* blackboard)
{
    (void)blackboard;
    elapsedSeconds_ = 0.0f;
}

void BTWaitAction::Reset()
{
    BossBTActionBase::Reset();
    elapsedSeconds_ = 0.0f;
}

void BTWaitAction::ApplyParameters(const nlohmann::json& params)
{
    BossBTActionBase::ApplyParameters(params);
    if (params.contains("waitBeats"))
    {
        waitBeats_ = params["waitBeats"].get<float>();
    }
}

nlohmann::json BTWaitAction::ExtractParameters() const
{
    nlohmann::json params = BossBTActionBase::ExtractParameters();
    params["waitBeats"] = waitBeats_;
    return params;
}

#ifdef _DEBUG
bool BTWaitAction::DrawImGui()
{
    bool changed = BossBTActionBase::DrawImGui();
    changed |= ImGui::DragFloat("Wait Beats", &waitBeats_, 0.1f, 0.1f, 32.0f);
    return changed;
}
#endif
