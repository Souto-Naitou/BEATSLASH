#include "ComboBuffSystem.h"

#include "ComboSystem.h"
#include <manager/BeatManager.h>
#include <judge/InputTimingJudge.h>
#include <Logger.h>

ComboBuffSystem::ComboBuffSystem(ComboSystem* comboSystem, InputTimingJudge* inputTimingJudge, BeatClock* beatClock):
    pComboSystem_(comboSystem),
    pInputTimingJudge_(inputTimingJudge),
    pBeatClock_(beatClock)
{
    if(!pComboSystem_ || !pInputTimingJudge_ || !pBeatClock_)
    {
        // いずれかのポインタが無効な場合はエラーを出力
    }
}

float ComboBuffSystem::GetDamageMultiplier() const
{
    return pComboSystem_->GetAttackPowerMultiplier();
}

void ComboBuffSystem::OnAttackHit()
{
    float timingDiff = pBeatClock_->GetDeltaToNearestBeatSeconds();
    JudgeResult judge = pInputTimingJudge_->Evaluate(timingDiff);
    static const char* judgeStr[] = { "Miss", "Good", "Perfect" };
    Tako::Logger::Log("ComboBuffSystem::OnAttackHit - timingDiff: {}, judge: {}\n", timingDiff, judgeStr[static_cast<int>(judge)]);
    pComboSystem_->OnAttackHit(judge);
}

void ComboBuffSystem::OnDamaged()
{
    pComboSystem_->OnDamage();
}
