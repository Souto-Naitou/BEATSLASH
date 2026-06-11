#pragma once
#include <BTNode.h>

class BeatClock;

/// <summary>
/// 拍同期の開始ゲートを持つボス行動ノードの基底クラス
/// syncStartToBeat_が有効な場合、拍境界のフレームまで開始を遅延させて行動の開始をリズムに合わせる
/// </summary>
class BossBTActionBase : public Tako::BTNode
{
public:
    void Reset() override;
    void ApplyParameters(const nlohmann::json& params) override;
    nlohmann::json ExtractParameters() const override;

#ifdef _DEBUG
    bool DrawImGui() override;
#endif

protected:
    /// <summary>
    /// 開始ゲートの更新
    /// 拍境界待ちの間はfalseを返す（呼び出し側はRunningを返すこと）。開始遷移したフレームでOnStartを呼ぶ
    /// </summary>
    bool UpdateStartGate(Tako::BTBlackboard* blackboard);

    // 開始ゲートを未開始状態へ戻す（Success返却時に呼ぶ）
    void ResetStartGate() { hasStarted_ = false; }

    bool HasStarted() const { return hasStarted_; }

    // 開始時の拍（拍同期時は整数拍へスナップ済み。クロック不在時は0）
    float GetStartBeat() const { return startBeat_; }

    // 開始フレームの初期化フック
    virtual void OnStart(Tako::BTBlackboard* blackboard) { (void)blackboard; }

private:
    // 拍境界で開始するか
    bool syncStartToBeat_ = true;
    bool hasStarted_ = false;
    float startBeat_ = 0.0f;
};
