#pragma once

#include <AABBCollider.h>
#include <Logger.h>
#include <utility/CollisionUtility.h>

class TestCollider : public Tako::OBBCollider
{
public:
        TestCollider() = default;
        ~TestCollider() = default;
        void OnCollisionEnter(Tako::Collider* other) override
        {
            //Tako::Logger::Log("ColEnter >> this : %d, other : %d", this->GetTypeID(),other->GetTypeID());
            Tako::Vector3 pushback = CollisionUtility::CalcPushback(this, other);
            
            GetTransform()->translate += pushback;
            
        }
        void OnCollisionStay(Tako::Collider* other) override
        {
            Tako::Vector3 pushback = CollisionUtility::CalcPushback(this, other);

            GetTransform()->translate += pushback;
        }
        void OnCollisionExit(Tako::Collider* other) override
        {
            //Tako::Logger::Log("ColExit >> this : %d, other : %d", this->GetTypeID(), other->GetTypeID());
        }


};