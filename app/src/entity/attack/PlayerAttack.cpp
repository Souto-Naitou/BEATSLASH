#include "PlayerAttack.h"
#include <utility>



PlayerAttack::PlayerAttack(ColliderRepository& colliderRepository)
{
    auto pAttackCollider = std::make_unique<PlayerAttackCollider>();
    pCollider_ = colliderRepository.AddCollider(std::move(pAttackCollider));
}

void PlayerAttack::Update(float deltaTime)
{
    colliderTimer_.Update(deltaTime);

    if (pCollider_ != nullptr)
    {
        // コライダーの無効化
        pCollider_->SetActive(colliderTimer_.IsActive());
        pCollider_ = nullptr;
    }
}
