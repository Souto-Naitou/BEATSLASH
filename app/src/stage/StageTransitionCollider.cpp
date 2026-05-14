#include "StageTransitionCollider.h"

void StageTransitionCollider::OnCollisionEnter(Tako::Collider* other)
{
    if (onTransitionTrigger_)
    {
        onTransitionTrigger_();
    }
}
