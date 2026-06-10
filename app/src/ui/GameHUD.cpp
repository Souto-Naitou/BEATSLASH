#include "GameHUD.h"

void GameHUD::Initialize()
{
    comboUI_ = std::make_unique<ComboUI>();
    comboUI_->Initialize();

    rhythmHintUI_ = std::make_unique<RhythmHintUI>();
    rhythmHintUI_->Initialize();

    playerHPBarUI_ = std::make_unique<PlayerHPBarUI>();
    playerHPBarUI_->Initialize();
}

void GameHUD::Update()
{
    comboUI_->Update(comboBuffSystem_.GetCurrentCombo());
    rhythmHintUI_->Update(beatClock_.GetDeltaToNearestBeat());
    playerHPBarUI_->Update(hpComponent_.GetHPRatio());
}

void GameHUD::Draw()
{
    comboUI_->Draw();
    rhythmHintUI_->Draw();
    playerHPBarUI_->Draw();
}
