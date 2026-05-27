#include "PlayerAttackHitReceiver.h"



void PlayerAttackHitReceiver::ReceiveHit(const HitInfo& info)
{
    // VFX
    // カメラ
    // コンボ更新
    execs_.comboBuffSystem.OnAttackHit();
}
