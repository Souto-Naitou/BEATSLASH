#pragma once
#include <cstdint>

enum class JudgeResult
{// TODO : 仮
    Miss,
    Hit,
    Critical
};

class ComboSystem
{
public:

    void OnAttackHit(JudgeResult judge);

    void OmDamage();

    int GetCombo() const;

    float GetAttackPowerMultiplier() const;

private:

    void AddCombo();

    void ResetCombo();

private:

    int32_t combo_ = 0; // 現在のコンボ数

};