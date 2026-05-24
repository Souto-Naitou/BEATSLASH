#include "StageTransitionCollider.h"

#include <type/ColliderTypeID.h>

void StageTransitionCollider::OnCollisionEnter(Tako::Collider* other)
{
    auto id = other->GetTypeID();
    if (id == static_cast<uint32_t>(ColliderTypeID::Player))
    {
        if (onTransitionTrigger_)
        {
            onTransitionTrigger_();
        }
    }

}
