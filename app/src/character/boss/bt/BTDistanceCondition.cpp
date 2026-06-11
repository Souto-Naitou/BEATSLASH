#include "BTDistanceCondition.h"
#include <BTBlackboard.h>
#include <character/boss/Boss.h>
#include <character/boss/bt/BossBlackboardKeys.h>

#ifdef _DEBUG
#include <imgui.h>
#endif

BTDistanceCondition::BTDistanceCondition()
{
    name_ = "DistanceCheck";
}

Tako::BTNodeStatus BTDistanceCondition::Execute(Tako::BTBlackboard* blackboard)
{
    const Boss* boss = blackboard->GetPtr<Boss>(BossBlackboardKeys::kBoss);
    const ICharacter* target = blackboard->GetPtr<const ICharacter>(BossBlackboardKeys::kTarget);
    if (!boss || !target)
    {
        status_ = Tako::BTNodeStatus::Failure;
        return status_;
    }

    Tako::Vector3 toTarget = target->GetPosition() - boss->GetPosition();
    toTarget.y = 0.0f;
    bool within = toTarget.Length() <= distance_;

    status_ = (within == checkWithin_) ? Tako::BTNodeStatus::Success : Tako::BTNodeStatus::Failure;
    return status_;
}

void BTDistanceCondition::ApplyParameters(const nlohmann::json& params)
{
    if (params.contains("distance"))
    {
        distance_ = params["distance"].get<float>();
    }
    if (params.contains("checkWithin"))
    {
        checkWithin_ = params["checkWithin"].get<bool>();
    }
}

nlohmann::json BTDistanceCondition::ExtractParameters() const
{
    return nlohmann::json{
        { "distance", distance_ },
        { "checkWithin", checkWithin_ },
    };
}

#ifdef _DEBUG
bool BTDistanceCondition::DrawImGui()
{
    bool changed = ImGui::DragFloat("Distance", &distance_, 0.1f, 0.1f, 100.0f);

    int mode = checkWithin_ ? 0 : 1;
    const char* modes[] = { "Within", "Beyond" };
    if (ImGui::Combo("Mode", &mode, modes, 2))
    {
        checkWithin_ = (mode == 0);
        changed = true;
    }
    return changed;
}
#endif
