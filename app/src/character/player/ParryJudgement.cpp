#include "ParryJudgement.h"



bool ParryJudgement::Judge() const
{
    // 最後のパリィからの経過時間を取得
    float timeSinceLastParry = history_.GetTimeSinceLastParry();

    // 経過時間がパリィ成功のウィンドウ内であれば成功と判定
    return timeSinceLastParry <= kParryWindow_;
}
