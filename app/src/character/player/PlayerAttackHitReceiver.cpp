#include "PlayerAttackHitReceiver.h"
#include <GPUParticle.h>




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
    // コンボ更新（ビート判定）
    JudgeResult judge = execs_.comboBuffSystem.OnAttackHit();
    execs_.onJudgeCallback(judge);

    /// [ VFX ]

    // ビートに乗ったヒット(Good以上)のみラジアルブラー発動
    if (judge != JudgeResult::Miss)
    {
        pRadialBeat_->Start(kTimeRadialBeat_);
    }

    // パーティクル発生
    execs_.playerAttackPresentation.OnHit();

    // カメラ
}
