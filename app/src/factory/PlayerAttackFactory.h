#pragma once
#include <memory>
#include <entity/attack/PlayerAttack.h>
#include <combo/ComboBuffSystem.h>
#include <EmitterManager.h>

class PlayerAttackFactory
{
public:
    struct Dependencies
    {
        ComboBuffSystem& comboBuffSystem;
        ColliderRepository& colliderRepository;
        Tako::EmitterManager& emitterManager;
    };

    PlayerAttackFactory(Dependencies& dependencies) ;

    std::unique_ptr<PlayerAttack> Create(const Tako::Vector3& position);


private:
    ComboBuffSystem& comboBuffSystem_;
    ColliderRepository& colliderRepository_;
    Tako::EmitterManager& emitterManager_;
};