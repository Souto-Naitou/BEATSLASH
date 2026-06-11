#pragma once
#include <BTNode.h>

/// <summary>
/// ターゲットとのXZ平面距離を判定する条件ノード
/// checkWithin_がtrueなら距離以内でSuccess、falseなら距離以上でSuccess
/// </summary>
class BTDistanceCondition : public Tako::BTNode
{
public:
    BTDistanceCondition();

    Tako::BTNodeStatus Execute(Tako::BTBlackboard* blackboard) override;
    void ApplyParameters(const nlohmann::json& params) override;
    nlohmann::json ExtractParameters() const override;

#ifdef _DEBUG
    bool DrawImGui() override;
#endif

private:
    // 判定するXZ平面距離のしきい値
    float distance_ = 10.0f;
    // true: しきい値以内でSuccess / false: しきい値以上でSuccess
    bool checkWithin_ = true;
};
