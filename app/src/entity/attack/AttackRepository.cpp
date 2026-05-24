#include "AttackRepository.h"



void AttackRepository::Update()
{
    for (auto& playerAttack : playerAttacks_)
    {
        playerAttack->Update(0.016f);
    }
}

void AttackRepository::CreatePlayerAttack(const Tako::Vector3& position, ComboBuffSystem* comboBuffSystem)
{
    auto pPlayerAttack = std::make_unique<PlayerAttack>(colliderRepository_, position, comboBuffSystem);
    playerAttacks_.emplace_back(std::move(pPlayerAttack));
}
