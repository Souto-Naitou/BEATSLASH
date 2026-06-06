#include "Overdrive.h"

#include <combo/ComboBuffSystem.h>


Overdrive::Overdrive(ComboBuffSystem* combobuffSystem):
    comboBuffSystem_(combobuffSystem)
{
}

void Overdrive::Activate()
{
    isActive_ = true;

    stopWatch_.Reset();
    stopWatch_.Start();
}

void Overdrive::Update()
{
    float elapse = stopWatch_.GetNow<float>();

    if (elapse >= duration_)
    {
        End();
    }
}

void Overdrive::End()
{
    isActive_ = false;
    stopWatch_.Reset();
}

void Overdrive::OnJudge(JudgeResult judge)
{
    if (!isActive_)
        return;

    if (judge == JudgeResult::Miss)
    {
        End();
        // コンボのリセット
        comboBuffSystem_->OnDamaged();
    }

}

float Overdrive::GetPowerUpRate() const
{
    return isActive_ ? attackPowerUpRate_ : 1.0f;
}

bool Overdrive::IsActive() const
{
    return isActive_;
}
