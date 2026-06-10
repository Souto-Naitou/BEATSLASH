#pragma once
#include "ParryHistory.h"
#include <debug/GameParameter.h>

class ParryJudgement
{
public:
    ParryJudgement(const ParryHistory& histroy) : history_(histroy) {}

    /// <summary>
    /// パリィ成功判定 (最後にパリィがトリガーされてから、現在までの経過時間が範囲内にあるかどうか)
    /// </summary>
    /// <returns>成否</returns>
    bool Judge() const;

private:
    EnableDebug("ParryJudgement");
    GameParameter(float, kParryWindow_, 0.5f); // パリィ成功と判定する時間ウィンドウ（秒）

    // パリィ履歴の参照
    const ParryHistory& history_;
};