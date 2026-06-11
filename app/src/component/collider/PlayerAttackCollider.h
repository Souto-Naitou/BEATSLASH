#pragma once
#include <OBBCollider.h>
#include <character/player/Player.h>
#include <character/player/PlayerAttackHitReceiver.h>

class ComboBuffSystem;

class PlayerAttackCollider : public Tako::OBBCollider
{
public:
    PlayerAttackCollider(PlayerAttackHitReceiver& hitReceiver) : hitReceiver_(hitReceiver) {}

    void OnCollisionEnter(Collider* other) override;


    void OnCollisionStay(Collider* other) override;


    void OnCollisionExit(Collider* other) override;


private:
    PlayerAttackHitReceiver& hitReceiver_;
};