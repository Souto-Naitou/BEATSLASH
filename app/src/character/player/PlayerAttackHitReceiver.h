#pragma once
#include <combo/ComboBuffSystem.h>
#include <memory>
#include <presentation/animation/RadialBeat.h>

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

    PlayerAttackHitReceiver(Executors& execs);

    void Update();

    void ReceiveHit(const HitInfo& info);

private:
    std::unique_ptr<RadialBeat> pRadialBeat_;
    Executors execs_;
};