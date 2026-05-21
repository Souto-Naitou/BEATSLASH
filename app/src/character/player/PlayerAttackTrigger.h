#pragma once
#include "PlayerInput.h"

/// 役割
/// - 入力を監視する
/// - クールタイムの管理
/// - コンボのカウント
/// - コライダーの生成要求
class PlayerAttackTrigger
{
public:
    void Update(const PlayerInput::PlayerCommand& command);
};