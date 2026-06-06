#include "PlayerAttackHitReceiver.h"
#include <GPUParticle.h>




PlayerAttackHitReceiver::PlayerAttackHitReceiver(Executors& execs) : execs_(execs)
{
    pRadialBeat_ = std::make_unique<RadialBeat>();
    pRadialBeat_->Initialize();
    pRadialBeat_->SetMaxWidth(0.03f);
}

void PlayerAttackHitReceiver::Update()
{
    pRadialBeat_->Update();
}

void PlayerAttackHitReceiver::ReceiveHit(const HitInfo& info)
{
    /// [ VFX ]
   
    // ビートエフェクト開始
    pRadialBeat_->Start(kTimeRadialBeat_);
    
    // パーティクル発生
    execs_.playerAttackPresentation.OnHit();

    // カメラ

    // コンボ更新
    execs_.comboBuffSystem.OnAttackHit();
}
