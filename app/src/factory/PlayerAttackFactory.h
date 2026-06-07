#pragma once
#include <memory>
#include <functional>
#include <entity/attack/PlayerAttack.h>
#include <combo/ComboBuffSystem.h>
#include <judge/JudgeResutl.h>
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

    PlayerAttackFactory(Dependencies& dependencies);

    std::unique_ptr<PlayerAttack> Create(const Tako::Vector3& position);

    void SetOnJudgeCallback(std::function<void(JudgeResult)> cb) { onJudge_ = std::move(cb); }

private:
    ComboBuffSystem& comboBuffSystem_;
    ColliderRepository& colliderRepository_;
    Tako::EmitterManager& emitterManager_;
    std::function<void(JudgeResult)> onJudge_;
};