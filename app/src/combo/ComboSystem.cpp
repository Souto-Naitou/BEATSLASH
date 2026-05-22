#include "ComboSystem.h"


void ComboSystem::OnAttackHit(JudgeResult judge)
{
    // ミスじゃないときはコンボを加算する
    if (judge != JudgeResult::Miss)
        AddCombo();
}

void ComboSystem::OnDamage()
{
    ResetCombo();
}

int ComboSystem::GetCombo() const
{
    return combo_;
}

float ComboSystem::GetAttackPowerMultiplier() const
{
    float multiplier = 1.0f;
    // コンボ数に応じて攻撃力の倍率を計算する
    return multiplier;
}

void ComboSystem::AddCombo()
{
    ++combo_;
}

void ComboSystem::ResetCombo()
{
    combo_ = 0;
}
