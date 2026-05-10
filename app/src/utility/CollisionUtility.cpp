#include "CollisionUtility.h"
#include <algorithm>
#include <cfloat>
#include <Logger.h>
#include <format>

using namespace Tako;

namespace CollisionUtility
{

// OBBvsOBB の SAT。衝突していない場合は penetration=0 の情報を返す
static CollisionInfo CalcSAT(const Tako::OBB& obbA, const Tako::OBB& obbB)
{
    Tako::Vector3 axesA[3] = { obbA.GetAxis(0), obbA.GetAxis(1), obbA.GetAxis(2) };
    Tako::Vector3 axesB[3] = { obbB.GetAxis(0), obbB.GetAxis(1), obbB.GetAxis(2) };
    Tako::Vector3 t = obbB.center - obbA.center;

    float minOverlap = FLT_MAX;
    Tako::Vector3 minAxis;

    // 1軸を検査し、分離軸なら false を返す。重なり量が最小なら記録する
    auto testAxis = [&](Tako::Vector3 axis) -> bool
        {
            float axisLen = axis.Length();
            if (axisLen < 0.0001f)
                return true;
            axis = axis / axisLen;

            float rA = obbA.halfExtents.x * std::abs(axesA[0].Dot(axis))
                + obbA.halfExtents.y * std::abs(axesA[1].Dot(axis))
                + obbA.halfExtents.z * std::abs(axesA[2].Dot(axis));
            float rB = obbB.halfExtents.x * std::abs(axesB[0].Dot(axis))
                + obbB.halfExtents.y * std::abs(axesB[1].Dot(axis))
                + obbB.halfExtents.z * std::abs(axesB[2].Dot(axis));

            float tProj = std::abs(t.Dot(axis));
            float overlap = rA + rB - tProj;
            if (overlap < 0.0f)
                return false;

            if (overlap < minOverlap)
            {
                minOverlap = overlap;
                // self(A) が other(B) から離れる方向を法線とする
                minAxis = (t.Dot(axis) >= 0.0f) ? -axis : axis;

                Logger::Log(std::format("minOverlap = {},rA = {},rB = {},tProj = {}\n", overlap, rA, rB, tProj));
            }
            return true;
        };

    for (int i = 0; i < 3; ++i)
    {
        if (!testAxis(axesA[i]))
            return CollisionInfo{};
    }
    for (int i = 0; i < 3; ++i)
    {
        if (!testAxis(axesB[i]))
            return CollisionInfo{};
    }
    for (int i = 0; i < 3; ++i)
    {
        for (int j = 0; j < 3; ++j)
        {
            if (!testAxis(axesA[i].Cross(axesB[j])))
                return CollisionInfo{};
        }
    }

    return { minAxis, minOverlap };
}

Tako::Vector3 CalcPushback(Tako::Collider* self, Tako::Collider* other)
{

    SphereCollider* sphereA = dynamic_cast<SphereCollider*>(self);
    SphereCollider* sphereB = dynamic_cast<SphereCollider*>(other);
    AABBCollider* aabbA = dynamic_cast<AABBCollider*>(self);
    AABBCollider* aabbB = dynamic_cast<AABBCollider*>(other);
    OBBCollider* obbA = dynamic_cast<OBBCollider*>(self);
    OBBCollider* obbB = dynamic_cast<OBBCollider*>(other);

    CollisionInfo collisionInfo;
    if (sphereA && sphereB)
    {
        collisionInfo = CalcCollisionInfo(sphereA, sphereB);
    }
    else if (sphereA && aabbB)
    {
        collisionInfo = CalcCollisionInfo(sphereA, aabbB);
    }
    else if (sphereA && obbB)
    {
        collisionInfo = CalcCollisionInfo(sphereA, obbB);
    }
    else if (aabbA && sphereB)
    {
        collisionInfo = CalcCollisionInfo(sphereB, aabbA);
        collisionInfo.contactNormal = -collisionInfo.contactNormal;
    }
    else if (aabbA && aabbB)
    {
        collisionInfo = CalcCollisionInfo(aabbA, aabbB);
    }
    else if (aabbA && obbB)
    {
        collisionInfo = CalcCollisionInfo(aabbA, obbB);
    }
    else if (obbA && sphereB)
    {
        collisionInfo = CalcCollisionInfo(sphereB, obbA);
        collisionInfo.contactNormal = -collisionInfo.contactNormal;
    }
    else if (obbA && aabbB)
    {
        collisionInfo = CalcCollisionInfo(aabbB, obbA);
        collisionInfo.contactNormal = -collisionInfo.contactNormal;
    }
    else if (obbA && obbB)
    {
        collisionInfo = CalcCollisionInfo(obbA, obbB);
    }
    else
    {
        // 対応していないコライダーの組み合わせ
        return Vector3();
    }

    // 押し戻し量
    Vector3 pushback = collisionInfo.contactNormal * collisionInfo.penetration;

    return pushback;
}

CollisionInfo CalcCollisionInfo(const Tako::SphereCollider* sphereA, const Tako::SphereCollider* sphereB)
{
    Vector3 distanceVec = sphereB->GetCenter() - sphereA->GetCenter();
    float distance = distanceVec.Length();
    float radiusSum = sphereA->GetRadius() + sphereB->GetRadius();

    CollisionInfo info;
    info.penetration = radiusSum - distance;
    // A が B から離れる方向 = B→A
    info.contactNormal = (distance < 0.0001f) ? Vector3(0.0f, 1.0f, 0.0f) : -(distanceVec / distance);

    return info;
}

CollisionInfo CalcCollisionInfo(const Tako::SphereCollider* sphere, const Tako::AABBCollider* aabb)
{
    Vector3 aabbMin = aabb->GetAABB().min;
    Vector3 aabbMax = aabb->GetAABB().max;
    Vector3 sphereCenter = sphere->GetCenter();

    Vector3 closestPoint(
        std::clamp(sphereCenter.x, aabbMin.x, aabbMax.x),
        std::clamp(sphereCenter.y, aabbMin.y, aabbMax.y),
        std::clamp(sphereCenter.z, aabbMin.z, aabbMax.z)
    );

    Vector3 distanceVec = sphereCenter - closestPoint;
    float distance = distanceVec.Length();
    Vector3 direction = distanceVec.Normalize();

    if (distance < 0.0001f)
    {
        // TODO : 接触方向の計算
        // 球の中心がAABB内部にあるときはより近い面の法線を使用するようにする
        // ここでは仮に上向きの法線を設定している
        direction = Vector3(0.0f, 1.0f, 0.0f); // 上向きの法線
    }

    CollisionInfo info;
    info.contactNormal = direction;
    info.penetration = sphere->GetRadius() - distance;

    return info;
}

CollisionInfo CalcCollisionInfo(const Tako::SphereCollider* sphere, const Tako::OBBCollider* obb)
{
    OBB obbData = obb->GetOBB();
    Vector3 sphereCenter = sphere->GetCenter();
    Vector3 axes[3] = { obbData.GetAxis(0), obbData.GetAxis(1), obbData.GetAxis(2) };
    float halfEx[3] = { obbData.halfExtents.x, obbData.halfExtents.y, obbData.halfExtents.z };

    // OBB 上の最近接点を求める
    Vector3 localCenter = sphereCenter - obbData.center;
    Vector3 closestPoint = obbData.center;
    for (int i = 0; i < 3; ++i)
    {
        float d = std::clamp(localCenter.Dot(axes[i]), -halfEx[i], halfEx[i]);
        closestPoint += axes[i] * d;
    }

    Vector3 distVec = sphereCenter - closestPoint;
    float distance = distVec.Length();

    CollisionInfo info;
    if (distance < 0.0001f)
    {
        // 球の中心が OBB 内部にある場合 → 最も近い面の法線を使う
        float minDist = FLT_MAX;
        for (int i = 0; i < 3; ++i)
        {
            float d = localCenter.Dot(axes[i]);
            float distToFace = halfEx[i] - std::abs(d);
            if (distToFace < minDist)
            {
                minDist = distToFace;
                info.contactNormal = (d >= 0.0f) ? axes[i] : -axes[i];
                info.penetration = sphere->GetRadius() + minDist;
            }
        }
    }
    else
    {
        info.contactNormal = distVec / distance;
        info.penetration = sphere->GetRadius() - distance;
    }

    return info;
}

CollisionInfo CalcCollisionInfo(const Tako::AABBCollider* aabbA, const Tako::AABBCollider* aabbB)
{
    Vector3 minA = aabbA->GetAABB().min;
    Vector3 maxA = aabbA->GetAABB().max;
    Vector3 minB = aabbB->GetAABB().min;
    Vector3 maxB = aabbB->GetAABB().max;

    Vector3 centerA = (minA + maxA) * 0.5f;
    Vector3 centerB = (minB + maxB) * 0.5f;

    // 各軸での重なりを計算
    Vector3 overlap(
        std::min(maxA.x, maxB.x) - std::max(minA.x, minB.x),
        std::min(maxA.y, maxB.y) - std::max(minA.y, minB.y),
        std::min(maxA.z, maxB.z) - std::max(minA.z, minB.z)
    );

    CollisionInfo info;

    // 最小の重なり軸を見つける
    if (overlap.x < overlap.y && overlap.x < overlap.z)
    {
        // X軸が最小の重なり
        info.penetration = overlap.x;
        info.contactNormal = (centerA.x < centerB.x) ? Vector3(-1.0f, 0.0f, 0.0f) : Vector3(1.0f, 0.0f, 0.0f);
    }
    else if (overlap.y < overlap.z)
    {
        // Y軸が最小の重なり
        info.penetration = overlap.y;
        info.contactNormal = (centerA.y < centerB.y) ? Vector3(0.0f, -1.0f, 0.0f) : Vector3(0.0f, 1.0f, 0.0f);
    }
    else
    {
        info.penetration = overlap.z;
        info.contactNormal = (centerA.z < centerB.z) ? Vector3(0.0f, 0.0f, -1.0f) : Vector3(0.0f, 0.0f, 1.0f);
    }

    return info;
}

CollisionInfo CalcCollisionInfo(const Tako::AABBCollider* aabb, const Tako::OBBCollider* obb)
{
    // AABB を回転なし OBB として扱い CalcSAT を再利用する
    AABB aabbData = aabb->GetAABB();
    OBB aabbAsOBB;
    aabbAsOBB.center = (aabbData.min + aabbData.max) * 0.5f;
    aabbAsOBB.halfExtents = (aabbData.max - aabbData.min) * 0.5f;
    aabbAsOBB.orientation = Mat4x4::MakeIdentity();

    return CalcSAT(aabbAsOBB, obb->GetOBB());
}

CollisionInfo CalcCollisionInfo(const Tako::OBBCollider* obbA, const Tako::OBBCollider* obbB)
{
    return CalcSAT(obbA->GetOBB(), obbB->GetOBB());
}

} // namespace CollisionUtility