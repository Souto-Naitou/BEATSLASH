#pragma once
#include <Transform.h>

class IMovement
{
public:
    virtual ~IMovement() = default;
    virtual void Update(Tako::Transform& transform, float deltaTime) = 0;
};