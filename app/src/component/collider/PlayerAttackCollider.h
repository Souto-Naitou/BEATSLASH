#pragma once
#include <OBBCollider.h>
#include <memory>

class ComboBuffSystem;

class PlayerAttackCollider : public Tako::OBBCollider
{
public:

    PlayerAttackCollider(ComboBuffSystem* comboBuffSystem) : pComboBuffSystem_(comboBuffSystem) {}

    void OnCollisionEnter(Collider* other) override;


    void OnCollisionStay(Collider* other) override;


    void OnCollisionExit(Collider* other) override;
private:

    ComboBuffSystem* pComboBuffSystem_ = nullptr; // コンボシステムへのポインタ

};