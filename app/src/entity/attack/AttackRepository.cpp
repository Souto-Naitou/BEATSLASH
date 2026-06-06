#include "AttackRepository.h"



void AttackRepository::Update()
{
    for (auto& playerAttack : playerAttacks_)
    {
        playerAttack->Update(0.016f);
    }
}

void AttackRepository::EraseInactiveAttacks()
{
    std::erase_if(playerAttacks_, [](const std::unique_ptr<PlayerAttack>& attack) {
        // 攻撃が非アクティブかどうかの条件をここに記述
        return !attack->IsActive();
    });
}

void AttackRepository::CreatePlayerAttack(const Tako::Vector3& position)
{
    auto attack = factories_.pPlayerAttackFactory->Create(position);
    playerAttacks_.emplace_back(std::move(attack));
}
