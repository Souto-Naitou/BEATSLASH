#include "PlayerAttack.h"
#include <utility>
#include <component/collider/PlayerAttackCollider.h>

#include <type/ColliderTypeID.h>

PlayerAttack::PlayerAttack(ColliderRepository& colliderRepository, const Tako::Vector3& position, ComboBuffSystem* comboBuffSystem)
{
    auto pAttackCollider = std::make_unique<PlayerAttackCollider>(comboBuffSystem);
    pCollider_ = colliderRepository.AddCollider(std::move(pAttackCollider));
    colliderTimer_.Enable(kColliderActiveTime_);

    transform_ = Tako::Transform(
        Tako::Vector3(1.0f, 1.0f, 1.0f),
        Tako::Vector3(0.0f, 0.0f, 0.0f),
        Tako::Vector3(1.0f, 1.0f, 1.0f)
    );

    transform_.translate = position;
    pCollider_->SetTransform(&transform_);

    pCollider_->SetTypeID(static_cast<uint32_t>(ColliderTypeID::PlayerAttack));
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
