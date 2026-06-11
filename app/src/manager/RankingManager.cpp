#include "RankingManager.h"

#include <algorithm>
#include <fstream>

RankingManager* RankingManager::GetInstance()
{
    static RankingManager instance;
    return &instance;
}

RankingManager::RankingManager()
{
    times_.fill(kEmpty);
    Load();
}

void RankingManager::AddTime(float seconds)
{
    lastAddedIndex_ = -1;

    // 5位（最下位）より良いタイムの場合のみ更新
    if (seconds >= times_[kMaxRank - 1]) return;

    times_[kMaxRank - 1] = seconds;
    std::sort(times_.begin(), times_.end());
    lastAddedIndex_ = static_cast<int>(std::find(times_.begin(), times_.end(), seconds) - times_.begin());
    Save();
}

void RankingManager::Save(const std::string& filePath)
{
    std::ofstream ofs(filePath, std::ios::binary);
    if (!ofs) return;
    ofs.write(reinterpret_cast<const char*>(times_.data()), sizeof(times_));
}

void RankingManager::Load(const std::string& filePath)
{
    std::ifstream ifs(filePath, std::ios::binary);
    if (!ifs) return;
    ifs.read(reinterpret_cast<char*>(times_.data()), sizeof(times_));
}
