#pragma once

#include <BaseScene.h>
#include <Sprite.h>
#include <Vector2.h>
#include <ozSound/audio/SoundEngine.h>
#include <utility/StopWatch.h>
#include <debug/GameParameter.h>
#include <presentation/animation/AnimationTimeline.hpp>
#include <memory>

class GameoverScene : public Tako::BaseScene
{
public:
    void Initialize() override;
    void Finalize() override;
    void Update() override;
    void Draw() override;
    void DrawWithoutEffect() override;
    void DrawImGui() override;

private:
    EnableDebug("GameoverScene");

    std::unique_ptr<Tako::Sprite> pSpriteTitle_;
    std::unique_ptr<Tako::Sprite> pToTitle_;
    std::unique_ptr<Tako::Sprite> pRetry_;

    AnimationTimeline<Tako::Vector2> titleTimeline_;
    AnimationTimeline<Tako::Vector2> toTitleTimeline_;
    AnimationTimeline<Tako::Vector2> retryTimeline_;


    ozSound::SoundHandle bgmHandle_ = ozSound::kInvalidHandle;
    float beatPhase_ = 0.0f;
    float loopTimer_ = 0.0f;
    
    bool postEventOnceFlag_ = true;

    // 低速フェード
    StopWatch musicFadeStopWatch_; // 音楽のフェードアウトの経過時間を計測するストップウォッチ
    GameParameter(float, musicFadeDuration_, 3.0f);
    GameParameter(float, musicMaxSpeed_, 1.0f);
    GameParameter(float, musicMinSpeed_, 0.0f);



    //static constexpr float kBPM = 150.0f;
    //static constexpr float kSecondsPerBeat = 60.0f / kBPM;
    //// なんビートで一回とするか
    //static constexpr int kBeatsPerPhase = 8;
};
