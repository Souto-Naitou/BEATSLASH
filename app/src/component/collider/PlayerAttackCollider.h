#pragma once
#include <OBBCollider.h>
#include <memory>
#include <physics/ColliderTimer.h>


class PlayerAttackCollider : public Tako::OBBCollider
{
public:

    void Update();


    void OnCollisionEnter(Collider* other) override;


    void OnCollisionStay(Collider* other) override;


    void OnCollisionExit(Collider* other) override;


private:
    std::unique_ptr<ColliderTimer> pColliderTimer_;
};