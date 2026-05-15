#pragma once

#include <character/base/Character.h>

class Player : public Character
{
public:
    void Initialize(CharacterColliderID colliderID) override;
    void Update() override;
    void Draw() override;

private:


};