#pragma once

#include <array>
#include <string>
#include <cfloat>

class RankingManager
{
public:
    static constexpr int   kMaxRank     = 5;
    static constexpr float kEmpty       = FLT_MAX;  // 未記録エントリのマーカー

    static RankingManager* GetInstance();

    // タイムを登録して上位 kMaxRank 件を昇順保持、自動セーブ
    void  AddTime(float seconds);

    const std::array<float, kMaxRank>& GetTimes() const { return times_; }

    void Save(const std::string& filePath = kDefaultPath);
    void Load(const std::string& filePath = kDefaultPath);

private:
    RankingManager();

    static constexpr const char* kDefaultPath = "resources/ranking.dat";

    std::array<float, kMaxRank> times_;
};
