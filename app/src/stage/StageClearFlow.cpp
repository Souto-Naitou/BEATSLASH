#include "StageClearFlow.h"
#include <Logger.h>
#include <CollisionManager.h>
#include <Input.h>
#include <type/ColliderTypeID.h>
void StageClearFlow::Initialize(const StageData& stageData)
{
    if (transitionCollider_)
    {
        Tako::CollisionManager::GetInstance()->RemoveCollider(transitionCollider_.get());
        transitionCollider_.reset();
    }

    transitionCollider_ = std::make_unique<StageTransitionCollider>();
    transform_ = stageData.transitionTransform;
    transitionCollider_->SetTransform(&transform_);
    transitionCollider_->SetSize(stageData.transitionTransform.scale); // scale をコライダーサイズとして使用
    transitionCollider_->SetOnTransitionTrigger([this]() { FadeOutEnter(); });
    transitionCollider_->SetTypeID(static_cast<int32_t>(ColliderTypeID::StageTransitionEvent)); // 適当な型IDを設定 TODO : enum 
    transitionCollider_->SetActive(false);

    Tako::CollisionManager::GetInstance()->AddCollider(transitionCollider_.get());
    Tako::CollisionManager::GetInstance()->SetCollisionMask(static_cast<int32_t>(ColliderTypeID::Player), static_cast<int32_t>(ColliderTypeID::StageTransitionEvent), true);

    if (!fade_)
    {
        fade_ = std::make_unique<Tako::FadeTransition>(Tako::Vector4(0, 0, 0, 1));
        fade_->Initialize();
    }
}

void StageClearFlow::Update(float deltaTime)
{
    switch (state_)
    {
        case StageFlowState::Playing:
            transitionCollider_->SetActive(false);
            break;
        case StageFlowState::StageClear:
            ClearUpdate(deltaTime);
            break;
        case StageFlowState::FadeIn:
            FadeInUpdate(deltaTime);
            break;
        case StageFlowState::FadeOut:
            FadeOutUpdate(deltaTime);
            break;
        default:
            break;
    }
}

void StageClearFlow::Draw()
{
    if (fade_)
    {
        fade_->Draw();
    }
}

void StageClearFlow::NotifyClear()
{
    ClearEnter();
}

void StageClearFlow::ActivateTransitionCollider()
{
    if (state_ == StageFlowState::StageClear)
        transitionCollider_->SetActive(true);
}

void StageClearFlow::ClearEnter()
{
    if (state_ != StageFlowState::Playing)
        return;

    state_ = StageFlowState::StageClear;
    //transitionCollider_->SetActive(true);
}

void StageClearFlow::FadeOutEnter()
{
    state_ = StageFlowState::FadeOut;
    fade_->Start(Tako::ITransitionEffect::TransitionState::FADE_OUT, fadeDuration_);
}

void StageClearFlow::FadeInEnter()
{
    state_ = StageFlowState::FadeIn;
    fade_->Start(Tako::ITransitionEffect::TransitionState::FADE_IN, fadeDuration_);
}

void StageClearFlow::ClearUpdate(float deltaTime)
{
    // トランジションへの遷移はコライダーがイベントを受け取ったときなどの条件で行う想定
}

void StageClearFlow::FadeOutUpdate(float deltaTime)
{
    fade_->Update();

    if (fade_->IsFinished())
    {
        if (onFadeOutComplete_)
        {
            onFadeOutComplete_();
        }
        FadeInEnter();
    }
}

void StageClearFlow::FadeInUpdate(float deltaTime)
{
    fade_->Update();
    if (fade_->IsFinished())
    {
        if (onFadeInComplete_)
        {
            onFadeInComplete_();
        }
        state_ = StageFlowState::Playing; // フェードイン完了後は通常プレイ状態に戻す
    }
}

void StageClearFlow::ClearExit()
{
}

void StageClearFlow::FadeOutExit()
{
}

void StageClearFlow::FadeInExit()
{
}