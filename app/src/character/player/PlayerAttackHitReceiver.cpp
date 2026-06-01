#include "PlayerAttackHitReceiver.h"
#include <GPUParticle.h>
#include <DebugUIManager.h>



PlayerAttackHitReceiver::PlayerAttackHitReceiver(Executors& execs) : execs_(execs)
{
    pRadialBeat_ = std::make_unique<RadialBeat>();
    pRadialBeat_->Initialize();
    pRadialBeat_->SetMaxWidth(0.03f);

    pEmitterManager_ = std::make_unique<Tako::EmitterManager>(Tako::GPUParticle::GetInstance());
    Tako::DebugUIManager::GetInstance()->SetEmitterManager(pEmitterManager_.get());
}

void PlayerAttackHitReceiver::Update()
{
    pRadialBeat_->Update();
}

void PlayerAttackHitReceiver::ReceiveHit(const HitInfo& info)
{
    // VFX
    pRadialBeat_->Start(kTimeRadialBeat_);

    // カメラ

    // コンボ更新
    JudgeResult judge = execs_.comboBuffSystem.OnAttackHit();
    execs_.overdrive.OnJudge(judge);
}
