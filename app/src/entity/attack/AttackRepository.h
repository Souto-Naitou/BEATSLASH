#pragma once
#include <vector>
#include <memory>
#include <functional>
#include <entity/attack/PlayerAttack.h>
#include <factory/PlayerAttackFactory.h>
#include <judge/JudgeResutl.h>
#include <common/PlayerAttackRequest.h>

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

    void CreatePlayerAttack(const PlayerAttackRequest& request);

    void SetOnJudgeCallback(std::function<void(JudgeResult)> cb)
    {
        factories_.pPlayerAttackFactory->SetOnJudgeCallback(std::move(cb));
    }

private:
    std::vector<std::unique_ptr<PlayerAttack>> playerAttacks_ = {};
    FactoryDependencies factories_;
};