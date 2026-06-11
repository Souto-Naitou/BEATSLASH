#include "TitleScene.h"
#include <SpriteBasic.h>
#include <utility/ViewportUnits.hpp>
#include <common/ResourcePath.h>
#include <TextureManager.h>
#include <transition/FadeTransition.h>
#include <SceneManager.h>
#include <Input.h>
#include <CollisionManager.h>
#include <utility/DeltaTimeManager.h>
#include <ShadowRenderer.h>
#include <Object3dBasic.h>
#include <math/Easing.h>

#ifdef _DEBUG
#include <DebugCamera.h>
#endif

void TitleScene::Initialize()
{
    #ifdef _DEBUG
    Tako::DebugCamera::GetInstance()->Initialize();
    Tako::Object3dBasic::GetInstance()->SetDebug(false);
    #endif // _DEBUG

    Tako::TextureManager::GetInstance()->LoadTexture(Global::ResourcePath::Texture::kTitleLogo);
    Tako::TextureManager::GetInstance()->LoadTexture(Global::ResourcePath::Texture::kTitleStartPrompt);

    pSpriteTitle_ = std::make_unique<Tako::Sprite>();
    pSpriteTitle_->Initialize(Global::ResourcePath::Texture::kTitleLogo);

    Tako::Vector2 titlePos = { 10_vw, 10_vh };
    pSpriteTitle_->SetPos(titlePos);


    pSpriteStartPrompt_ = std::make_unique<Tako::Sprite>();
    pSpriteStartPrompt_->Initialize(Global::ResourcePath::Texture::kTitleStartPrompt);
    pSpriteStartPrompt_->SetAnchorPoint({ 0.5f, 0.5f });

    Tako::Vector2 promptPos = { 50_vw, 80_vh };
    pSpriteStartPrompt_->SetPos(promptPos);

    pStage_ = std::make_unique<StageSequence>();
    pStage_->Initialize("resources/stage/StageData.json");

    pRailCamera_ = std::make_unique<RailCamera>();
    pRailCamera_->Initialize(kCenter_.GetPtr());
    Tako::Object3dBasic::GetInstance()->SetCamera(pRailCamera_->GetCamera());

    AnimationTween<Tako::Vector3> tween1(0.0f, 10.0f, { 0.0f, 20.0f, -30.0f }, { 0.0f, 20.0f, 30.0f });
    AnimationTween<Tako::Vector3> tween2(10.0f, 20.0f, { 0.0f, 20.0f, 30.0f }, { 0.0f, 20.0f, -30.0f });
    tween1.SetTransitionFunction(Math::Easing::EaseInOutCubic);
    tween2.SetTransitionFunction(Math::Easing::EaseInOutCubic);
    railCameraTimeline_.AddTween(tween1);
    railCameraTimeline_.AddTween(tween2);
}

void TitleScene::Finalize()
{
    Tako::CollisionManager::GetInstance()->Reset();
}

void TitleScene::Update()
{
    const float deltaTime = DeltaTimeManager::GetInstance()->GetDeltaTime(DeltaTimeChannelReserved::Game);

    if (!railCameraTimeline_.IsPlaying())
    {
        railCameraTimeline_.Start();
    }

    pRailCamera_->Update(railCameraTimeline_, deltaTime);

    // ステージの更新
    pStage_->Update(deltaTime);

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
    auto pShadowRenderer = Tako::ShadowRenderer::GetInstance();
    if (pShadowRenderer->IsEnabled())
    {
        pShadowRenderer->BeginShadowPass();
        pStage_->Draw();
        pShadowRenderer->EndShadowPass();
    }

    /// 3dモデルの描画
    Tako::Object3dBasic::GetInstance()->SetCommonRenderSetting();
    pStage_->Draw();

    /// 前景スプライトの描画
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
