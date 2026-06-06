#include "PlayerAttack.h"
#include <utility>
#include <component/collider/PlayerAttackCollider.h>
#include <character/player/PlayerAttackHitReceiver.h>

#include <type/ColliderTypeID.h>

PlayerAttack::PlayerAttack(InitData& initData)
{
    // プレゼンテーションの生成
    pPresentation_ = std::make_unique<PlayerAttackPresentation>(initData.emitterManager);

    // ヒット受信クラスの生成
    PlayerAttackHitReceiver::Executors receiverExecs =
    {
        .comboBuffSystem = initData.comboBuffSystem,
        .playerAttackPresentation = *pPresentation_
    };
    pHitReceiver_ = std::make_unique<PlayerAttackHitReceiver>(receiverExecs);

    // コライダーの生成・リポジトリに登録
    auto pAttackCollider = std::make_unique<PlayerAttackCollider>(*pHitReceiver_);
    pCollider_ = initData.colliderRepository.AddCollider(std::move(pAttackCollider));
    transform_ = Tako::Transform(
        Tako::Vector3(1.0f, 1.0f, 1.0f),
        Tako::Vector3(0.0f, 0.0f, 0.0f),
        initData.position
    );
    pCollider_->SetTransform(&transform_);
    pCollider_->SetTypeID(static_cast<uint32_t>(ColliderTypeID::PlayerAttack));

    // コライダーの有効時間を設定
    colliderTimer_.Enable(kColliderActiveTime_);

    pPresentation_->SetColliderPositionRef(&transform_.translate);

}

void PlayerAttack::Update(float deltaTime)
{
    colliderTimer_.Update(deltaTime);

    if (pCollider_ && !colliderTimer_.IsActive())
    {
        // コライダーの無効化
        pCollider_->SetActive(false);
        pCollider_ = nullptr;
    }

    if (pCollider_)
    {
        transform_.translate = pCollider_->GetCenter();
    }

    pHitReceiver_->Update();
    pPresentation_->Update();
}
