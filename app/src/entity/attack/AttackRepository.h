#pragma once
#include <vector>
#include <memory>
#include <entity/attack/PlayerAttack.h>
#include <physics/ColliderRepository.h>

class ComboSystem;

/// 攻撃のリポジトリクラス
/// 時間の関係でファクトリーの作成を見送る
class AttackRepository
{
public:
    AttackRepository(ColliderRepository& c) : colliderRepository_(c) {}

    /// <summary>
    /// 攻撃の更新
    /// </summary>
    void Update();

    void EraseInactiveAttacks();

    void CreatePlayerAttack(const Tako::Vector3& position, ComboBuffSystem* comboBuffSystem);

private:
    std::vector<std::unique_ptr<PlayerAttack>> playerAttacks_ = {};
    ColliderRepository& colliderRepository_;
};