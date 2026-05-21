#pragma once
#include <vector>
#include <physics/ColliderTimer.h>

class PlayerAttack
{
public:
    void Update();


private:
    std::vector<ColliderTimer> colliderTimers_;
};