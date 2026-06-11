#pragma once
#include <character/boss/bt/BossBTActionBase.h>

/// <summary>
/// ターゲットへ一定距離まで接近するアクションノード
/// XZ平面距離がstopDistance_以下になるまで移動し続ける（打ち切りなし）
/// </summary>
class BTApproachAction : public BossBTActionBase
{
public:
    BTApproachAction();

    Tako::BTNodeStatus Execute(Tako::BTBlackboard* blackboard) override;
    void ApplyParameters(const nlohmann::json& params) override;
    nlohmann::json ExtractParameters() const override;

#ifdef _DEBUG
    bool DrawImGui() override;
#endif

private:
    // 接近を終了するターゲットとのXZ平面距離
    float stopDistance_ = 3.0f;
    // 移動速度 [unit/s]
    float moveSpeed_ = 4.0f;
};
