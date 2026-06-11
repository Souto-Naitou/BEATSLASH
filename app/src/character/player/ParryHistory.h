#pragma once
#include <queue>
#include <chrono>

#include <utility/StopWatch.h>
#include <manager/BeatClock.h>
#include <Vector3.h>


/// パリィの履歴を保持するクラス
class ParryHistory
{
public:
    ParryHistory(const BeatClock& beatClock);
    ~ParryHistory();

    // パリィが発生した時間を記録
    void Record(const Tako::Vector3& position);
    // 最新のヒットからの経過時間を秒単位で取得
    float GetTimeSinceLastParry() const;
    // ヒットの履歴をクリア
    inline void Clear() { parryTimes_.clear(); }

    void ImGui();

private:

    // ヒットした時間のキュー
    std::vector<std::chrono::steady_clock::time_point> parryTimes_ = {};
    // ビートクロックへの参照
    const BeatClock& beatClock_;

    /// クールダウン用
    static constexpr float  kNumParryCooldownBeat_  = 1.2f; // クールダウンとする拍数
    StopWatch               stopWatch_              = {};   // ストップウォッチ
};
