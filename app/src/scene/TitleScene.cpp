#include "TitleScene.h"
#include <SpriteBasic.h>
#include <utility/ViewportUnits.hpp>
#include <common/ResourcePath.h>
#include <TextureManager.h>
#include <transition/FadeTransition.h>
#include <SceneManager.h>
#include <Input.h>
#include <CollisionManager.h>


void TitleScene::Initialize()
{
    Tako::TextureManager::GetInstance()->LoadTexture(Global::ResourcePath::Texture::kTitleLogo);
    Tako::TextureManager::GetInstance()->LoadTexture(Global::ResourcePath::Texture::kTitleStartPrompt);

    pSpriteTitle_ = std::make_unique<Tako::Sprite>();
    pSpriteTitle_->Initialize(Global::ResourcePath::Texture::kTitleLogo);

    Tako::Vector2 titlePos = { 10_vw, 10_vh };
    pSpriteTitle_->SetPos(titlePos);


    pSpriteStartPrompt_ = std::make_unique<Tako::Sprite>();
    pSpriteStartPrompt_->Initialize(Global::ResourcePath::Texture::kTitleStartPrompt);

    Tako::Vector2 promptPos = { 50_vw, 80_vh };
    pSpriteStartPrompt_->SetPos(promptPos);
}

void TitleScene::Finalize()
{
}

void TitleScene::Update()
{
    pSpriteTitle_->Update();
    pSpriteStartPrompt_->Update();

    if (Tako::Input::GetInstance()->TriggerKey(DIK_SPACE))
    {
        Tako::SceneManager::GetInstance()->ChangeScene("game", Tako::TransitionManager::EffectType::Fade, 1.0f);
    }
    auto& colls = Tako::CollisionManager::GetInstance()->GetColliders();
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
