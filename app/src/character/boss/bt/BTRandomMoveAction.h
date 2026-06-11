#pragma once
#include <character/boss/bt/BossBTActionBase.h>
#include <Vector3.h>

/// <summary>
/// XZ平面上のランダムな地点へ向かって移動するアクションノード
/// 目的地到達または累計移動距離がmoveDistance_に達したら成功終了する
/// Y座標は変更せず、重力との干渉を避ける
/// </summary>
class BTRandomMoveAction : public BossBTActionBase
{
public:
    BTRandomMoveAction();

    Tako::BTNodeStatus Execute(Tako::BTBlackboard* blackboard) override;
    void Reset() override;
    void ApplyParameters(const nlohmann::json& params) override;
    nlohmann::json ExtractParameters() const override;

#ifdef _DEBUG
    bool DrawImGui() override;
#endif

protected:
    void OnStart(Tako::BTBlackboard* blackboard) override;

private:
    Tako::Vector3 PickRandomDestination(const Tako::Vector3& origin) const;

private:
    // 到達判定の許容距離
    static constexpr float kArriveEpsilon = 0.1f;

    // 目的地を選ぶ半径（XZ平面）
    float moveRange_ = 8.0f;
    // 移動速度 [unit/s]
    float moveSpeed_ = 4.0f;
    // この距離だけ移動したら目的地未到達でも終了する
    float moveDistance_ = 4.0f;
    // 今回の実行で移動した累計距離
    float movedDistance_ = 0.0f;
    Tako::Vector3 destination_{};
    bool hasDestination_ = false;
};
