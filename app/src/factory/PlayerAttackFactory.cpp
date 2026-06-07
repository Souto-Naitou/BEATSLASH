#include "PlayerAttackFactory.h"



PlayerAttackFactory::PlayerAttackFactory(Dependencies& dependencies) :
    comboBuffSystem_(dependencies.comboBuffSystem),
    colliderRepository_(dependencies.colliderRepository),
    emitterManager_(dependencies.emitterManager)
{
}

std::unique_ptr<PlayerAttack> PlayerAttackFactory::Create(const Tako::Vector3& position)
{
    // 必要なパラメータをセット
    PlayerAttack::InitData initData =
    {
        .colliderRepository = colliderRepository_,
        .comboBuffSystem = comboBuffSystem_,
        .position = position,
        .emitterManager = emitterManager_,
        .onJudge = onJudge_
    };

    return std::make_unique<PlayerAttack>(initData);
}

