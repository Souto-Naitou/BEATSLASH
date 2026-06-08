#pragma once

class Player;

struct PlayerStateContext
{
    // ここにプレイヤーの状態遷移に必要な情報を追加する
    // 例:
    // PlayerInput& input;
    // PlayerMovement& movement;
    // PlayerAttackTrigger& attackTrigger;
    // PlayerAttackHitReceiver& attackHitReceiver;
    Player& player;
};