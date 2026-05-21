#include "ColliderRepository.h"



Tako::Collider* ColliderRepository::AddCollider()
{
    return colliders_.emplace_back(std::make_unique<Tako::Collider>()).get();
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
