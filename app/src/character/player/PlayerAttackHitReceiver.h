#pragma once
#include <combo/ComboBuffSystem.h>

class PlayerAttackHitReceiver
{
public:
    struct HitInfo
    {

    };

    struct Executors
    {
        // コンボシステム
        ComboBuffSystem& comboBuffSystem;
    };

    PlayerAttackHitReceiver(Executors& execs) : execs_(execs) {};

    void ReceiveHit(const HitInfo& info);

private:
    Executors& execs_;
};