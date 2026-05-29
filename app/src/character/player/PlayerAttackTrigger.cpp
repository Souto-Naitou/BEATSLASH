#include "PlayerAttackTrigger.h"



void PlayerAttackTrigger::UpdateCooldown(float deltaTime)
{
    if (remainingCooldownTime_ > 0.0f)
    {
        remainingCooldownTime_ -= deltaTime;
        if (remainingCooldownTime_ < 0.0f)
        {
            remainingCooldownTime_ = 0.0f;
        }
    }
}

bool PlayerAttackTrigger::ShouldAttack(const PlayerInput::PlayerCommand& command)
{
    // 攻撃ボタンが押されているか
    bool isAttackKeyTriggered = command.isAttackTriggered;

    // クールタイムが終了しているか
    bool isCooldownOver = remainingCooldownTime_ <= 0.0f;

    if (isAttackKeyTriggered && isCooldownOver)
    {
        // 攻撃がトリガーされたらクールタイムをリセット
        remainingCooldownTime_ = cooldownTime_;
    }

    // 攻撃がトリガーされていて、クールタイムが終了している場合に攻撃を実行
    return isAttackKeyTriggered && isCooldownOver;
}
