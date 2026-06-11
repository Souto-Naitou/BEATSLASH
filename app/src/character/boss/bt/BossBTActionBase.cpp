#include "BossBTActionBase.h"
#include <BTBlackboard.h>
#include <manager/BeatClock.h>
#include <character/boss/bt/BossBlackboardKeys.h>
#include <cmath>

#ifdef _DEBUG
#include <imgui.h>
#endif

bool BossBTActionBase::UpdateStartGate(Tako::BTBlackboard* blackboard)
{
    if (hasStarted_)
    {
        return true;
    }

    const BeatClock* beatClock = blackboard->GetPtr<const BeatClock>(BossBlackboardKeys::kBeatClock);
    if (syncStartToBeat_ && beatClock)
    {
        // 拍境界のフレームまで開始を遅延させる
        if (!blackboard->GetAs<bool>(BossBlackboardKeys::kBeatPassed, false))
        {
            return false;
        }
        // 境界フレームでは拍が僅かに進んでいるため整数拍へスナップする
        startBeat_ = std::floor(beatClock->GetCurrentBeat());
    }
    else
    {
        startBeat_ = beatClock ? beatClock->GetCurrentBeat() : 0.0f;
    }

    hasStarted_ = true;
    OnStart(blackboard);
    return true;
}

void BossBTActionBase::Reset()
{
    BTNode::Reset();
    ResetStartGate();
}

void BossBTActionBase::ApplyParameters(const nlohmann::json& params)
{
    if (params.contains("syncStartToBeat"))
    {
        syncStartToBeat_ = params["syncStartToBeat"].get<bool>();
    }
}

nlohmann::json BossBTActionBase::ExtractParameters() const
{
    return nlohmann::json{ { "syncStartToBeat", syncStartToBeat_ } };
}

#ifdef _DEBUG
bool BossBTActionBase::DrawImGui()
{
    return ImGui::Checkbox("Sync Start To Beat", &syncStartToBeat_);
}
#endif
