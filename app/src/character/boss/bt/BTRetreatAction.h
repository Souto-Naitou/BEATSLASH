#pragma once
#include <character/boss/bt/BossBTActionBase.h>

/// <summary>
/// ターゲットから一定距離離れるアクションノード
/// XZ平面距離がretreatDistance_以上になるか、累計移動距離がmoveDistance_に達したら成功終了する
/// </summary>
class BTRetreatAction : public BossBTActionBase
{
public:
    BTRetreatAction();

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
    // 離脱方向を正規化できないとみなすターゲットとの最小距離
    static constexpr float kMinDirectionDistance = 0.001f;

    // 離脱を終了するターゲットとのXZ平面距離
    float retreatDistance_ = 10.0f;
    // 移動速度 [unit/s]
    float moveSpeed_ = 4.0f;
    // この距離だけ移動したら目標距離未達でも終了する
    float moveDistance_ = 8.0f;
    // 今回の実行で移動した累計距離
    float movedDistance_ = 0.0f;
};
