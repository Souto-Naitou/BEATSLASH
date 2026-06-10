#include "GameHUD.h"

GameHUD::GameHUD(const ComboBuffSystem& comboBuffSystem, const BeatClock& beatClock) :
    comboBuffSystem_(comboBuffSystem),
    beatClock_(beatClock)
{
}

void GameHUD::Initialize()
{
    comboUI_ = std::make_unique<ComboUI>();
    comboUI_->Initialize();

    rhythmHintUI_ = std::make_unique<RhythmHintUI>();
    rhythmHintUI_->Initialize();
}

void GameHUD::Update()
{
    comboUI_->Update(comboBuffSystem_.GetCurrentCombo());
    rhythmHintUI_->Update(beatClock_.GetDeltaToNearestBeat());
}

void GameHUD::Draw()
{
    comboUI_->Draw();
    rhythmHintUI_->Draw();
}
