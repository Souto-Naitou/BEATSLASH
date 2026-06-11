#include "ParryPresentation.h"
#include <common/ParticleEmitterPresetNames.h>
#include <functional>
#include <system/EventListener.h>


ParryPresentation::ParryPresentation(Tako::EmitterManager& emitterManager) : emitterManager_(emitterManager)
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
    uniqueNameParryEffect_ = std::string(Global::ParticleEmitterPresetNames::kParrySuccess) + '_' + std::to_string(uniqueIndexSuccess_++);
    emitterManager_.CreateTemporaryEmitterFrom(Global::ParticleEmitterPresetNames::kParrySuccess, uniqueNameParryEffect_, 0.3f);
    emitterManager_.SetEmitterActive(uniqueNameParryEffect_, true);
    emitterManager_.SetEmitterPosition(uniqueNameParryEffect_, position);
}

void ParryPresentation::Update()
{

}

void ParryPresentation::OnParryReject(const Event::PlayerParry::Rejected& param)
{
    std::string uniqueNameParryFailEffect = std::string(Global::ParticleEmitterPresetNames::kParryFail) + "_" + std::to_string(uniqueIndexFail_++);
    emitterManager_.CreateTemporaryEmitterFrom(Global::ParticleEmitterPresetNames::kParryFail, uniqueNameParryFailEffect, 0.3f);
    emitterManager_.SetEmitterActive(uniqueNameParryFailEffect, true);
    emitterManager_.SetEmitterPosition(uniqueNameParryFailEffect, param.position);
}

void ParryPresentation::OnparryTrue(const Event::PlayerParry::True& param)
{
    std::string uniqueNameParryTrueEffect = std::string(Global::ParticleEmitterPresetNames::kParryTrue) + "_" + std::to_string(uniqueIndexTrue_++);
    emitterManager_.CreateTemporaryEmitterFrom(Global::ParticleEmitterPresetNames::kParryTrue, uniqueNameParryTrueEffect, 0.3f);
    emitterManager_.SetEmitterActive(uniqueNameParryTrueEffect, true);
    emitterManager_.SetEmitterPosition(uniqueNameParryTrueEffect, param.position);
}
