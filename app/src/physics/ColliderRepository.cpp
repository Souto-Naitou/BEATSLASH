#include "ColliderRepository.h"
#include <utility>



Tako::Collider* ColliderRepository::AddCollider(std::unique_ptr<Tako::Collider> pCollider)
{
    return colliders_.emplace_back(std::move(pCollider)).get();
}

void ColliderRepository::RemoveIfNotActive()
{
    auto onColliderNotActive = [this](const std::unique_ptr<Tako::Collider>& collider) {
        if (!collider->IsActive())
        {
            pCollisionManager_->RemoveCollider(collider.get());
            return true;
        }
        return false;
    };

    std::erase_if(colliders_, onColliderNotActive);
}
