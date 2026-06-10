#include "BTRandomMoveAction.h"
#include <BTBlackboard.h>
#include <character/boss/Boss.h>
#include <character/boss/bt/BossBlackboardKeys.h>
#include <cmath>
#include <numbers>
#include <random>

#ifdef _DEBUG
#include <imgui.h>
#endif

BTRandomMoveAction::BTRandomMoveAction()
{
    name_ = "RandomMove";
}

Tako::BTNodeStatus BTRandomMoveAction::Execute(Tako::BTBlackboard* blackboard)
{
    Boss* boss = blackboard->GetPtr<Boss>(BossBlackboardKeys::kBoss);
    if (!boss)
    {
        status_ = Tako::BTNodeStatus::Failure;
        return status_;
    }

    Tako::Transform& transform = boss->GetTransform();

    if (!hasDestination_)
    {
        destination_ = PickRandomDestination(transform.translate);
        hasDestination_ = true;
    }

    Tako::Vector3 toDest = destination_ - transform.translate;
    toDest.y = 0.0f;
    float distance = toDest.Length();

    if (distance <= kArriveEpsilon)
    {
        hasDestination_ = false;
        status_ = Tako::BTNodeStatus::Success;
        return status_;
    }

    float step = moveSpeed_ * blackboard->GetDeltaTime();
    Tako::Vector3 direction = toDest.Normalize();

    // 行き過ぎ防止のため残距離でクランプする
    if (step >= distance)
    {
        transform.translate.x = destination_.x;
        transform.translate.z = destination_.z;
        hasDestination_ = false;
        status_ = Tako::BTNodeStatus::Success;
        return status_;
    }

    transform.translate += direction * step;
    // 進行方向へ向きを合わせる
    transform.rotate.y = std::atan2(direction.x, direction.z);

    status_ = Tako::BTNodeStatus::Running;
    return status_;
}

Tako::Vector3 BTRandomMoveAction::PickRandomDestination(const Tako::Vector3& origin) const
{
    static thread_local std::mt19937 engine{ std::random_device{}() };
    std::uniform_real_distribution<float> angleDist(0.0f, 2.0f * std::numbers::pi_v<float>);
    std::uniform_real_distribution<float> radiusDist(0.0f, moveRange_);

    float angle = angleDist(engine);
    float radius = radiusDist(engine);
    return { origin.x + std::cos(angle) * radius, origin.y, origin.z + std::sin(angle) * radius };
}

void BTRandomMoveAction::Reset()
{
    BTNode::Reset();
    hasDestination_ = false;
}

void BTRandomMoveAction::ApplyParameters(const nlohmann::json& params)
{
    if (params.contains("moveRange"))
    {
        moveRange_ = params["moveRange"].get<float>();
    }
    if (params.contains("moveSpeed"))
    {
        moveSpeed_ = params["moveSpeed"].get<float>();
    }
}

nlohmann::json BTRandomMoveAction::ExtractParameters() const
{
    return nlohmann::json{
        { "moveRange", moveRange_ },
        { "moveSpeed", moveSpeed_ },
    };
}

#ifdef _DEBUG
bool BTRandomMoveAction::DrawImGui()
{
    bool changed = false;
    changed |= ImGui::DragFloat("Move Range", &moveRange_, 0.1f, 0.5f, 100.0f);
    changed |= ImGui::DragFloat("Move Speed", &moveSpeed_, 0.1f, 0.1f, 50.0f);
    return changed;
}
#endif
