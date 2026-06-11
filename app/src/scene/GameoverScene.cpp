#include "GameoverScene.h"

#include <Input.h>
#include <SceneManager.h>
#include <FrameTimer.h>
#include <PostEffectManager.h>
#include <PostEffectStruct.h>
#include <SpriteBasic.h>
#include <TextureManager.h>
#include <utility/ViewportUnits.hpp>
#include <ozSound/audio/AudioSystem.h>

#include <math/Easing.h>
#include <presentation/animation/AnimationTween.hpp>
#include <cmath>

namespace
{
    constexpr const char* kTexTitle  = "gameover/gameover.png";
    constexpr const char* kBackToTitle = "gameover/back_to_title.png";
    constexpr const char* kRetry = "gameover/retry.png";
}

void GameoverScene::Initialize()
{
    auto* se = ozSound::SoundEngine::GetInstance();
    se->StopAll();
    bgmHandle_ = se->Play("bgm_game_0", 0.3f, true);

    // --- スプライト ---
    Tako::TextureManager::GetInstance()->LoadTexture(kTexTitle);
    Tako::TextureManager::GetInstance()->LoadTexture(kBackToTitle);
    Tako::TextureManager::GetInstance()->LoadTexture(kRetry);

    pSpriteTitle_ = std::make_unique<Tako::Sprite>();
    pSpriteTitle_->Initialize(kTexTitle);
    pSpriteTitle_->SetAnchorPoint({ 0.5f, 0.5f });

    pToTitle_ = std::make_unique<Tako::Sprite>();
    pToTitle_->Initialize(kBackToTitle);
    pToTitle_->SetAnchorPoint({ 0.5f, 0.5f });

    pRetry_ = std::make_unique<Tako::Sprite>();
    pRetry_->Initialize(kRetry);
    pRetry_->SetAnchorPoint({ 0.5f, 0.5f });

    // タイトル: 上からスライドイン
    {
        const Tako::Vector2 start = { 50_vw, -10_vh };
        const Tako::Vector2 end   = { 50_vw, 35_vh };
        AnimationTween<Tako::Vector2> tween(1.0f, 1.7f, start, end);
        tween.SetTransitionFunction(Math::Easing::EaseOutBack);
        titleTimeline_.AddTween(tween);
        titleTimeline_.Start(start);
    }
    // タイトルへ: 下からスライドイン (0.4s後)
    {
        const Tako::Vector2 start = { 50_vw, 120_vh };
        const Tako::Vector2 end   = { 50_vw, 72_vh };
        AnimationTween<Tako::Vector2> tween(2.4f, 1.5f, start, end);
        tween.SetTransitionFunction(Math::Easing::EaseOutCubic);
        toTitleTimeline_.AddTween(tween);
        toTitleTimeline_.Start(start);
    }
    // リトライ: 下からスライドイン (0.6s後)
    {
        const Tako::Vector2 start = { 50_vw, 120_vh };
        const Tako::Vector2 end   = { 50_vw, 80_vh };
        AnimationTween<Tako::Vector2> tween(2.6f, 1.5f, start, end);
        tween.SetTransitionFunction(Math::Easing::EaseOutCubic);
        retryTimeline_.AddTween(tween);
        retryTimeline_.Start(start);
    }

    musicFadeStopWatch_.Start();

    beatPhase_ = 0.0f;
    postEventOnceFlag_ = true;
}

void GameoverScene::Finalize()
{
    ozSound::SoundEngine::GetInstance()->Stop(bgmHandle_);
    ozSound::AudioSystem::GetInstance()->GetBGMSubmix()->ClearFilter();
    ozSound::SoundEngine::GetInstance()->PostEvent("stop_bgm_gameover");
    //Tako::PostEffectManager::GetInstance()->RemoveEffectFromChain("Vignette");
}

void GameoverScene::Update()
{
    const float deltaTime = Tako::FrameTimer::GetInstance()->GetDeltaTime();

    auto* se = ozSound::SoundEngine::GetInstance();

    /* fade */
    float fadeProgress = musicFadeStopWatch_.GetNow<float>() / musicFadeDuration_;
    float musicSpeed = musicMaxSpeed_ * (1.0f - fadeProgress) + musicMinSpeed_ * fadeProgress;
    if (fadeProgress >= 1.0f && bgmHandle_ != ozSound::kInvalidHandle)
    {
        se->Stop(bgmHandle_);
        bgmHandle_ = ozSound::kInvalidHandle;
    }
    if (bgmHandle_ != ozSound::kInvalidHandle)
        se->SetPlaySpeed(bgmHandle_, musicSpeed);

    // フェードアウト完了後にゲームオーバーBGMを再生
    static bool once = true;
    if (fadeProgress > 2.0f && postEventOnceFlag_)
    {
        se->PostEvent("play_bgm_gameover");
        postEventOnceFlag_ = false;
    }

    /*beatPhase_ += deltaTime / kSecondsPerBeat / static_cast<float>(kBeatsPerPhase);
    if (beatPhase_ >= 1.0f) 
        beatPhase_ -= 1.0f;

    const float pulse = std::pow(std::sinf(beatPhase_ * 3.14159265f), 4.0f);

    Tako::VignetteParam vp{};
    vp.power = 0.7f + pulse * 0.6f;
    vp.range = 0.35f;
    vp.color = { 0.2f, 0.2f, 0.2f };
    Tako::PostEffectManager::GetInstance()->SetEffectParam("Vignette", vp);*/

    loopTimer_ += deltaTime;

    Tako::Vector2 titlePos = titleTimeline_.Update();
    if (!titleTimeline_.IsPlaying())
    {
        titlePos.y += std::sinf(loopTimer_ * 3.14159265f) * 1.5_vh;
    }
    pSpriteTitle_->SetPos(titlePos);
    pToTitle_->SetPos(toTitleTimeline_.Update());
    pRetry_->SetPos(retryTimeline_.Update());

    pSpriteTitle_->Update();
    pToTitle_->Update();
    pRetry_->Update();

    if (Tako::Input::GetInstance()->TriggerKey(DIK_SPACE))
    {
        Tako::SceneManager::GetInstance()->ChangeScene("game", Tako::TransitionManager::EffectType::Fade, 0.5f);
    }
    if (Tako::Input::GetInstance()->TriggerKey(DIK_ESCAPE))
    {
        Tako::SceneManager::GetInstance()->ChangeScene("title", Tako::TransitionManager::EffectType::Fade, 0.5f);
    }
}

void GameoverScene::Draw()
{
    Tako::SpriteBasic::GetInstance()->SetCommonRenderSetting();

    pSpriteTitle_->Draw();
    pToTitle_->Draw();
    pRetry_->Draw();
}

void GameoverScene::DrawWithoutEffect()
{
}

void GameoverScene::DrawImGui()
{
}
