#pragma once
#include <cstdint>
#include <judge/JudgeResutl.h>

class ComboSystem
{
public:

    void OnAttackHit(JudgeResult judge);

    void OnDamage();

    int GetCombo() const;

    float GetAttackPowerMultiplier() const;

private:

    void AddCombo();

    void ResetCombo();

private:
    
    int32_t combo_ = 0; // 現在のコンボ数

};