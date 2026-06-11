#include "ParryPresentation.h"
#include <common/ParticleEmitterPresetNames.h>
#include <utility/DeltaTimeManager.h>
#include <functional>
#include <system/EventListener.h>


ParryPresentation::ParryPresentation(Tako::EmitterManager& emitterManager, const Tako::Vector3& playerPosition)
    : emitterManager_(emitterManager), playerPosition_(playerPosition)
{
    emitterManager_.SetEmitterActive(Global::ParticleEmitterPresetNames::kParrySuccess, false);
    emitterManager_.SetEmitterActive(Global::ParticleEmitterPresetNames::kParryTrue, false);
    emitterManager_.SetEmitterActive(Global::ParticleEmitterPresetNames::kParryFail, false);
    subsRejectParry_ = EventListener::GetInstance()->Subscribe<Event::PlayerParry::Rejected>(
        std::bind(&ParryPresentation::OnParryReject,
            this,
            std::placeholders::_1
            )
    );
    subsTrueParry_ = EventListener::GetInstance()->Subscribe<Event::PlayerParry::True>(
        std::bind(&ParryPresentation::OnparryTrue,
            this,
            std::placeholders::_1
            )
    );
}

void ParryPresentation::Play(const Tako::Vector3& position)
{
    std::string uniqueNameParryEffect = std::string(Global::ParticleEmitterPresetNames::kParrySuccess) + '_' + std::to_string(uniqueIndexSuccess_++);
    emitterManager_.CreateTemporaryEmitterFrom(Global::ParticleEmitterPresetNames::kParrySuccess, uniqueNameParryEffect, kEffectLifeTime_);
    emitterManager_.SetEmitterActive(uniqueNameParryEffect, true);
    emitterManager_.SetEmitterPosition(uniqueNameParryEffect, position);
    activeEffects_.push_back({ std::move(uniqueNameParryEffect), kEffectLifeTime_, position - playerPosition_ });
}

void ParryPresentation::Update()
{
    if (activeEffects_.empty()) return;

    const float deltaTime = DeltaTimeManager::GetInstance()->GetDeltaTime(DeltaTimeChannelReserved::Game);

    // 生存中のエミッターをプレイヤー位置に追従させ、寿命切れはリストから外す
    std::erase_if(activeEffects_, [&](ActiveEffect& effect)
    {
        effect.remainingTime -= deltaTime;
        if (effect.remainingTime <= 0.0f) return true;

        emitterManager_.SetEmitterPosition(effect.name, playerPosition_ + effect.offset);
        return false;
    });
}

void ParryPresentation::OnParryReject(const Event::PlayerParry::Rejected& param)
{
    std::string uniqueNameParryFailEffect = std::string(Global::ParticleEmitterPresetNames::kParryFail) + "_" + std::to_string(uniqueIndexFail_++);
    emitterManager_.CreateTemporaryEmitterFrom(Global::ParticleEmitterPresetNames::kParryFail, uniqueNameParryFailEffect, kEffectLifeTime_);
    emitterManager_.SetEmitterActive(uniqueNameParryFailEffect, true);
    emitterManager_.SetEmitterPosition(uniqueNameParryFailEffect, param.position);
    activeEffects_.push_back({ std::move(uniqueNameParryFailEffect), kEffectLifeTime_, param.position - playerPosition_ });
}

void ParryPresentation::OnparryTrue(const Event::PlayerParry::True& param)
{
    std::string uniqueNameParryTrueEffect = std::string(Global::ParticleEmitterPresetNames::kParryTrue) + "_" + std::to_string(uniqueIndexTrue_++);
    emitterManager_.CreateTemporaryEmitterFrom(Global::ParticleEmitterPresetNames::kParryTrue, uniqueNameParryTrueEffect, kEffectLifeTime_);
    emitterManager_.SetEmitterActive(uniqueNameParryTrueEffect, true);
    emitterManager_.SetEmitterPosition(uniqueNameParryTrueEffect, param.position);
    activeEffects_.push_back({ std::move(uniqueNameParryTrueEffect), kEffectLifeTime_, param.position - playerPosition_ });
}
