#include "ParryPresentation.h"
#include <common/ParticleEmitterPresetNames.h>

ParryPresentation::ParryPresentation(Tako::EmitterManager& emitterManager) : emitterManager_(emitterManager)
{
    emitterManager_.SetEmitterActive(Global::ParticleEmitterPresetNames::kParrySuccess, false);
}

void ParryPresentation::Play(const Tako::Vector3& position)
{
    uniqueNameParryEffect_ = std::string(Global::ParticleEmitterPresetNames::kParrySuccess) + '_' + std::to_string(uniqueIndex_++);
    emitterManager_.CreateTemporaryEmitterFrom(Global::ParticleEmitterPresetNames::kParrySuccess, uniqueNameParryEffect_, 0.3f);
    emitterManager_.SetEmitterActive(uniqueNameParryEffect_, true);
    emitterManager_.SetEmitterPosition(uniqueNameParryEffect_, position);
}

void ParryPresentation::Update()
{

}
