#include "PlayerAttackHitReceiver.h"
#include <GPUParticle.h>


#include <system/GameEvent.h>
#include <system/EventListener.h>


PlayerAttackHitReceiver::PlayerAttackHitReceiver(Executors& execs) : execs_(execs)
{
    pRadialBeat_ = std::make_unique<RadialBeat>();
    pRadialBeat_->Initialize();
    pRadialBeat_->SetMaxWidth(0.03f);
}

PlayerAttackHitReceiver::~PlayerAttackHitReceiver()
{
    pRadialBeat_->Finalize();
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
    JudgeResult judge = execs_.comboBuffSystem.OnAttackHit();
    execs_.onJudgeCallback(judge);

    float damage = damageAmount_;
    if (judge == JudgeResult::Perfect)
        damage *= execs_.comboBuffSystem.GetDamageMultiplier();
    else if (judge == JudgeResult::Good)
        damage *= execs_.comboBuffSystem.GetDamageMultiplier() * 0.8f;
    else
        damage *= execs_.comboBuffSystem.GetDamageMultiplier() * 0.3f;
    EventListener::GetInstance()->Publish(EnemyDamageEvent{ damage });
}
