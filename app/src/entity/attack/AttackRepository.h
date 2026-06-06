#pragma once
#include <vector>
#include <memory>
#include <entity/attack/PlayerAttack.h>
#include <factory/PlayerAttackFactory.h>

class ComboSystem;

/// 攻撃のリポジトリクラス
class AttackRepository
{
public:
    struct FactoryDependencies
    {
        PlayerAttackFactory* pPlayerAttackFactory;
    };

    AttackRepository(const FactoryDependencies& factories) : factories_(factories) {}

    /// <summary>
    /// 攻撃の更新
    /// </summary>
    void Update();

    void EraseInactiveAttacks();

    void CreatePlayerAttack(const Tako::Vector3& position);

private:
    std::vector<std::unique_ptr<PlayerAttack>> playerAttacks_ = {};
    FactoryDependencies factories_;
};