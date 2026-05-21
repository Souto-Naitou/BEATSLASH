#pragma once
#include <vector>
#include <memory>
#include <entity/attack/PlayerAttack.h>
#include <physics/ColliderRepository.h>

/// 攻撃のリポジトリクラス
/// 時間の関係でファクトリーの作成を見送る
class AttackRepository
{
public:
    AttackRepository(ColliderRepository& c) : colliderRepository_(c) {}

    void Update();

    void CreatePlayerAttack(const Tako::Vector3& position);

private:
    std::vector<std::unique_ptr<PlayerAttack>> playerAttacks_;
    ColliderRepository& colliderRepository_;
};