#include "Player.h"



void Player::Initialize(CharacterColliderID colliderID)
{
    this->Character::Initialize(colliderID);
}

void Player::Update()
{
    this->Character::Update();
}

void Player::Draw()
{
    this->Character::Draw();
}
