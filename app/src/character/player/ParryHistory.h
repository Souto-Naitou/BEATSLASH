#pragma once
#include <queue>
#include <chrono>



/// パリィの履歴を保持するクラス
class ParryHistory
{
public:
    ParryHistory();
    ~ParryHistory();

    // パリィが発生した時間を記録
    void Record();
    // 最新のヒットからの経過時間を秒単位で取得
    float GetTimeSinceLastParry() const;
    // ヒットの履歴をクリア
    inline void Clear() { parryTimes_.clear(); }

    void ImGui();

private:
    // ヒットした時間のキュー
    std::vector<std::chrono::steady_clock::time_point> parryTimes_;
};
