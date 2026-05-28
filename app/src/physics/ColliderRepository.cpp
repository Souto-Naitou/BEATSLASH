#include "ColliderRepository.h"
#include <utility>
#include <CollisionManager.h>



Tako::Collider* ColliderRepository::AddCollider(std::unique_ptr<Tako::Collider> pCollider)
{
    const auto& newCollider = colliders_.emplace_back(std::move(pCollider)).get();
    Tako::CollisionManager::GetInstance()->AddCollider(newCollider);
    return newCollider;
}

void ColliderRepository::EraseInactiveColliders()
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
