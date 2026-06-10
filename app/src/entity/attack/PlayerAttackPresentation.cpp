#include "PlayerAttackPresentation.h"
#include <common/ParticleEmitterPresetNames.h>


std::unordered_map<std::string, uint32_t> PlayerAttackPresentation::emitterIndexMap_ = {
    { Global::ParticleEmitterPresetNames::kTrail, 0 },
    { Global::ParticleEmitterPresetNames::kTrailBlackBlue, 0 },
    { Global::ParticleEmitterPresetNames::kShort, 0 }
};

PlayerAttackPresentation::PlayerAttackPresentation(Tako::EmitterManager& emitterManager) : emitterManager_(emitterManager)
{
    emitterManager_.SetEmitterActive(Global::ParticleEmitterPresetNames::kTrail, false);
    emitterManager_.SetEmitterActive(Global::ParticleEmitterPresetNames::kShort, false);
    emitterManager_.SetEmitterActive(Global::ParticleEmitterPresetNames::kTrailBlackBlue, false);

    /// トレイルの初期エミッターを作成しておく（位置は毎フレーム更新）
    auto& indexTrail = emitterIndexMap_.at(Global::ParticleEmitterPresetNames::kTrail);
    uniqueNameTrail_ = std::string(Global::ParticleEmitterPresetNames::kTrail) + '_' + std::to_string(indexTrail++);
    emitterManager_.CreateTemporaryEmitterFrom(Global::ParticleEmitterPresetNames::kTrail, uniqueNameTrail_, 3.0f);
    emitterManager_.SetEmitterActive(uniqueNameTrail_, true);

    auto& indexTrailBlackBlue = emitterIndexMap_.at(Global::ParticleEmitterPresetNames::kTrailBlackBlue);
    uniqueNameTrailBB_ = std::string(Global::ParticleEmitterPresetNames::kTrailBlackBlue) + '_' + std::to_string(indexTrailBlackBlue++);
    emitterManager_.CreateTemporaryEmitterFrom(Global::ParticleEmitterPresetNames::kTrailBlackBlue, uniqueNameTrailBB_, 3.0f);
    emitterManager_.SetEmitterActive(uniqueNameTrailBB_, true);
}

PlayerAttackPresentation::~PlayerAttackPresentation()
{
    emitterManager_.SetEmitterActive(uniqueNameTrail_, false);
    emitterManager_.SetEmitterActive(uniqueNameTrailBB_, false);
}

void PlayerAttackPresentation::Update()
{
    auto trailEmitter = emitterManager_.GetEmitterByName(uniqueNameTrail_);
    if (trailEmitter)
    {
        emitterManager_.SetEmitterPosition(uniqueNameTrail_, *pPositionCollider_);
        trailEmitter->BindTargetPosition(pPositionCollider_);
    }

    auto trailBBEmitter = emitterManager_.GetEmitterByName(uniqueNameTrailBB_);
    if (trailBBEmitter)
    {
        emitterManager_.SetEmitterPosition(uniqueNameTrailBB_, *pPositionCollider_);
        trailBBEmitter->BindTargetPosition(pPositionCollider_);
    }
}

void PlayerAttackPresentation::OnHit()
{
    auto& index = emitterIndexMap_.at(Global::ParticleEmitterPresetNames::kShort);
    std::string uniqueName = Global::ParticleEmitterPresetNames::kShort + '_' + std::to_string(index++);
    emitterManager_.CreateTemporaryEmitterFrom(Global::ParticleEmitterPresetNames::kShort, uniqueName, 0.5f);
    emitterManager_.SetEmitterPosition(uniqueName, *pPositionCollider_);
    emitterManager_.SetEmitterActive(uniqueName, true);
}
