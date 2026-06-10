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
    const BeatClock* beatClock = blackboard->GetPtr<const BeatClock>(BossBlackboardKeys::kBeatClock);

    if (beatClock)
    {
        if (!isStarted_)
        {
            isStarted_ = true;
            startBeat_ = beatClock->GetCurrentBeat();
        }

        if (beatClock->GetCurrentBeat() - startBeat_ >= waitBeats_)
        {
            isStarted_ = false;
            status_ = Tako::BTNodeStatus::Success;
            return status_;
        }
    }
    else
    {
        if (!isStarted_)
        {
            isStarted_ = true;
            elapsedSeconds_ = 0.0f;
        }

        elapsedSeconds_ += blackboard->GetDeltaTime();
        if (elapsedSeconds_ >= waitBeats_)
        {
            isStarted_ = false;
            status_ = Tako::BTNodeStatus::Success;
            return status_;
        }
    }

    status_ = Tako::BTNodeStatus::Running;
    return status_;
}

void BTWaitAction::Reset()
{
    BTNode::Reset();
    isStarted_ = false;
    elapsedSeconds_ = 0.0f;
}

void BTWaitAction::ApplyParameters(const nlohmann::json& params)
{
    if (params.contains("waitBeats"))
    {
        waitBeats_ = params["waitBeats"].get<float>();
    }
}

nlohmann::json BTWaitAction::ExtractParameters() const
{
    return nlohmann::json{ { "waitBeats", waitBeats_ } };
}

#ifdef _DEBUG
bool BTWaitAction::DrawImGui()
{
    return ImGui::DragFloat("Wait Beats", &waitBeats_, 0.1f, 0.1f, 32.0f);
}
#endif
