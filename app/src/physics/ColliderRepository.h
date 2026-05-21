#pragma once
#include <memory>
#include <vector>
#include <Collider.h>
#include <CollisionManager.h>


class ColliderRepository
{
public:
    Tako::Collider* AddCollider();
    void RemoveIfNotActive();
    void Clear() { colliders_.clear(); }

private:
    std::vector<std::unique_ptr<Tako::Collider>> colliders_;
    Tako::CollisionManager* pCollisionManager_ = Tako::CollisionManager::GetInstance();
};