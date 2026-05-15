#include "PhysicsMovement.h"

void PhysicsMovement::Update(Tako::Transform& transform, float deltaTime)
{
    this->PositionUpdate(transform.translate, deltaTime);
}

void PhysicsMovement::AddImpulse(const Tako::Vector3& impulse)
{
    velocity_ += impulse;
}

void PhysicsMovement::StopMovement()
{
    velocity_ = Tako::Vector3();
    acceleration_ = Tako::Vector3();
}

void PhysicsMovement::PositionUpdate(Tako::Vector3& position, float deltaTime)
{
    velocity_ += acceleration_ * deltaTime;
    position += velocity_ * deltaTime;
    acceleration_ = Tako::Vector3();
}
