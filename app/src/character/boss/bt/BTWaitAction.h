#pragma once
#include <BTNode.h>

/// <summary>
/// 指定拍数だけ待機するアクションノード
/// BeatClock不在時はwaitBeats_を秒として扱う
/// </summary>
class BTWaitAction : public Tako::BTNode
{
public:
    BTWaitAction();

    Tako::BTNodeStatus Execute(Tako::BTBlackboard* blackboard) override;
    void Reset() override;
    void ApplyParameters(const nlohmann::json& params) override;
    nlohmann::json ExtractParameters() const override;

#ifdef _DEBUG
    bool DrawImGui() override;
#endif

private:
    // 待機する拍数（BeatClock不在時は秒）
    float waitBeats_ = 1.0f;
    // 実行開始時の拍
    float startBeat_ = 0.0f;
    // BeatClock不在時のフォールバック用経過秒数
    float elapsedSeconds_ = 0.0f;
    bool isStarted_ = false;
};
