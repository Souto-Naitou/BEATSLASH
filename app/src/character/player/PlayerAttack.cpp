#include "PlayerAttack.h"



void PlayerAttack::Update()
{
    for (auto& colliderTimer : colliderTimers_)
    {
        colliderTimer.UpdateTime();
    }
}
