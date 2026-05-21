#include "PlayerAttack.h"
#include <utility>
#include <component/collider/PlayerAttackCollider.h>



PlayerAttack::PlayerAttack(ColliderRepository& colliderRepository, const Tako::Vector3& position)
{
    auto pAttackCollider = std::make_unique<PlayerAttackCollider>();
    pCollider_ = colliderRepository.AddCollider(std::move(pAttackCollider));
    colliderTimer_.Enable(kColliderActiveTime_);

    transform_.translate = position;
    pCollider_->SetTransform(&transform_);
}

void PlayerAttack::Update(float deltaTime)
{
    colliderTimer_.Update(deltaTime);

    if (colliderTimer_.IsActive())
    {
        // コライダーの無効化
        pCollider_->SetActive(false);
        pCollider_ = nullptr;
    }
}
