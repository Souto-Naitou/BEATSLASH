#pragma once
#include <Vector3.h>

#include <Collider.h>
#include <SphereCollider.h>
#include <AABBCollider.h>
#include <OBBCollider.h>

namespace CollisionUtility
{

struct CollisionInfo
{
    Tako::Vector3 contactNormal;    // 衝突面の法線
    float penetration = 0.0f;       // めりこみ量
};


Tako::Vector3 CalcPushback(Tako::Collider* self, Tako::Collider* other);

CollisionInfo CalcCollisionInfo(const Tako::SphereCollider* sphereA, const Tako::SphereCollider* sphereB);
CollisionInfo CalcCollisionInfo(const Tako::SphereCollider* sphere, const Tako::AABBCollider* aabb);
CollisionInfo CalcCollisionInfo(const Tako::SphereCollider* sphere, const Tako::OBBCollider* obb);

CollisionInfo CalcCollisionInfo(const Tako::AABBCollider* aabbA, const Tako::AABBCollider* aabbB);
CollisionInfo CalcCollisionInfo(const Tako::AABBCollider* aabb, const Tako::OBBCollider* obb);

CollisionInfo CalcCollisionInfo(const Tako::OBBCollider* obbA, const Tako::OBBCollider* obbB);

}
