#include "PlayerAttackHitReceiver.h"



PlayerAttackHitReceiver::PlayerAttackHitReceiver(Executors& execs) : execs_(execs)
{
    pRadialBeat_ = std::make_unique<RadialBeat>();
    pRadialBeat_->Initialize();
    pRadialBeat_->SetMaxWidth(0.5f);
}

void PlayerAttackHitReceiver::Update()
{
    pRadialBeat_->Update();
}

void PlayerAttackHitReceiver::ReceiveHit(const HitInfo& info)
{
    // VFX
    pRadialBeat_->Start(2.0f);

    // カメラ

    // コンボ更新
    execs_.comboBuffSystem.OnAttackHit();
}
