#include "TitleScene.h"
#include <SpriteBasic.h>
#include <utility/ViewportUnits.hpp>


void TitleScene::Initialize()
{
    pSpriteTitle_ = std::make_unique<Tako::Sprite>();
    pSpriteTitle_->Initialize("title/logo.png");



    pSpriteStartPrompt_ = std::make_unique<Tako::Sprite>();
    pSpriteStartPrompt_->Initialize("title/start_prompt.png");
}

void TitleScene::Finalize()
{
}

void TitleScene::Update()
{
    pSpriteTitle_->Update();
    pSpriteStartPrompt_->Update();
}

void TitleScene::Draw()
{
    Tako::SpriteBasic::GetInstance()->SetCommonRenderSetting();
    pSpriteTitle_->Draw();
    pSpriteStartPrompt_->Draw();
}

void TitleScene::DrawWithoutEffect()
{
}

void TitleScene::DrawImGui()
{
}
