#include "PlayerAttackTrigger.h"
#include <component/collider/PlayerAttackCollider.h>



void PlayerAttackTrigger::Update(const PlayerInput::PlayerCommand& command)
{
    if (command.isAttackTriggered)
    {
        /// コライダーの生成
        auto pAttackCollider = std::make_unique<PlayerAttackCollider>();
        /// コライダーリポジトリに追加
    }
}
