#pragma once
#include <combo/ComboBuffSystem.h>
#include <memory>
#include <presentation/animation/RadialBeat.h>
#include <debug\GameParameter.h>
#include <skill/Overdrive.h>
#include <EmitterManager.h>

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
        // オーバードライブ
        Overdrive& overdrive;
    };

    PlayerAttackHitReceiver(Executors& execs);

    void Update();

    void ReceiveHit(const HitInfo& info);

private:
    EnableDebug("PlayerAttackHitReceiver");

    GameParameter(float, kTimeRadialBeat_, 0.3f);

    std::unique_ptr<RadialBeat> pRadialBeat_;
    Executors execs_;
    std::unique_ptr<Tako::EmitterManager> pEmitterManager_ = nullptr;
};