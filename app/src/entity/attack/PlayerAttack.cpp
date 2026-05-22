#include "PlayerAttack.h"
#include <utility>
#include <component/collider/PlayerAttackCollider.h>



PlayerAttack::PlayerAttack(ColliderRepository& colliderRepository, const Tako::Vector3& position)
{
    auto pAttackCollider = std::make_unique<PlayerAttackCollider>();
    pCollider_ = colliderRepository.AddCollider(std::move(pAttackCollider));
    colliderTimer_.Enable(kColliderActiveTime_);

    transform_ = Tako::Transform(
        Tako::Vector3(1.0f, 1.0f, 1.0f),
        Tako::Vector3(0.0f, 0.0f, 0.0f),
        Tako::Vector3(1.0f, 1.0f, 1.0f)
    );

    transform_.translate = position;
    pCollider_->SetTransform(&transform_);
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
}
