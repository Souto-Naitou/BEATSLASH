#include "GameHUD.h"

GameHUD::GameHUD(const ComboBuffSystem& comboBuffSystem) :
    comboBuffSystem_(comboBuffSystem)
{
}

void GameHUD::Initialize()
{
    comboUI_ = std::make_unique<ComboUI>();
    comboUI_->Initialize();
}

void GameHUD::Update()
{
    comboUI_->Update(comboBuffSystem_.GetCurrentCombo());
}

void GameHUD::Draw()
{
    comboUI_->Draw();
}
