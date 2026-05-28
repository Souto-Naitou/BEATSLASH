#include "ComboBuffSystem.h"

#include "ComboSystem.h"
#include <manager/BeatManager.h>
#include <judge/InputTimingJudge.h>

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

uint32_t ComboBuffSystem::GetCurrentCombo() const
{
    return pComboSystem_->GetCombo();
}

void ComboBuffSystem::OnAttackHit()
{
    float timingDiff = pBeatClock_->GetDeltaToNearestBeatSeconds();
    JudgeResult judge = pInputTimingJudge_->Evaluate(timingDiff);
    pComboSystem_->OnAttackHit(judge);
}

void ComboBuffSystem::OnDamaged()
{
    pComboSystem_->OnDamage();
}
