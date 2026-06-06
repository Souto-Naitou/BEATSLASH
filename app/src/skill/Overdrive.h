#pragma once

#include "IPlayerSkill.h"
#include "utility/StopWatch.h"
#include <judge/JudgeResutl.h>

class ComboBuffSystem;

class Overdrive : public IPlayerSkill
{

public:

    Overdrive(ComboBuffSystem* combobuffSystem);
    ~Overdrive() override = default;

    void Activate() override;
    void Update() override;
    void End() override;

    void OnJudge(JudgeResult judge);
    float GetPowerUpRate() const;

    bool IsActive() const override;

private:

    ComboBuffSystem* comboBuffSystem_ = nullptr;

    StopWatch stopWatch_={};

    // 短く、高倍率
    // 長いとその分コンボを途切れさせるリスクが高まってしまうため、短めに設定

    float duration_ = 10.0f;

    float attackPowerUpRate_ = 1.5f;

    bool isActive_ = false;

};
