#pragma once
#include <OBBCollider.h>
#include <memory>


class PlayerAttackCollider : public Tako::OBBCollider
{
public:
    void OnCollisionEnter(Collider* other) override;


    void OnCollisionStay(Collider* other) override;


    void OnCollisionExit(Collider* other) override;
};