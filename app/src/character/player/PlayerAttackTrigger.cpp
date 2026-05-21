#include "PlayerAttackTrigger.h"



bool PlayerAttackTrigger::ShouldAttack(const PlayerInput::PlayerCommand& command)
{
    // 攻撃ボタンが押されているか
    bool isAttackKeyTriggered = command.isAttackTriggered;
    // クールタイムが終了しているか
    bool isCooldownOver = true; // TODO: クールタイムの管理を実装する

    return isAttackKeyTriggered && isCooldownOver;
}
