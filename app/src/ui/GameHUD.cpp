#include "GameHUD.h"

void GameHUD::Initialize()
{
    comboUI_ = std::make_unique<ComboUI>();
    comboUI_->Initialize();

    rhythmHintUI_ = std::make_unique<RhythmHintUI>();
    rhythmHintUI_->Initialize();

    playerHPBarUI_ = std::make_unique<HPBarUI>("Player");
    playerHPBarUI_->Initialize();

    bossHPBarUI_ = std::make_unique<HPBarUI>("Boss");
    bossHPBarUI_->Initialize();

    controlKeyUI_ = std::make_unique<ControlKeyUI>();
    controlKeyUI_->Initialize(playerInput_.IsGamepadMode());
}

void GameHUD::Update()
{
    comboUI_->Update(comboBuffSystem_.GetCurrentCombo());
    rhythmHintUI_->Update(beatClock_.GetDeltaToNearestBeat());
    playerHPBarUI_->Update(playerHPComponent_.GetHPRatio());
    controlKeyUI_->Update(playerInput_.IsGamepadMode());

    if (pBossHPComponent_)
        bossHPBarUI_->Update(pBossHPComponent_->GetHPRatio());
}

void GameHUD::Draw()
{
    comboUI_->Draw();
    rhythmHintUI_->Draw();
    playerHPBarUI_->Draw();
    controlKeyUI_->Draw();

    if (pBossHPComponent_)
        bossHPBarUI_->Draw();
}
