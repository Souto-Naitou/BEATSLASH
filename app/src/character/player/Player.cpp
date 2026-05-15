#include "Player.h"



void Player::Initialize(CharacterColliderID colliderID)
{
    this->Character::Initialize(colliderID);
    this->Character::SetScale({ 1.0f, 1.0f, 1.0f });
    this->Character::SetPosition({ 0.0f, 2.0f, 0.0f });
}

void Player::Update()
{
    this->Character::Update();
}

void Player::Draw()
{
    this->Character::Draw();
}
