#include "StageClearFlow.h"
#include <Logger.h>
#include <CollisionManager.h>
#include <Input.h>

void StageClearFlow::Initialize(const StageData& stageData)
{
    if (transitionCollider_)
        transitionCollider_.release();

    transitionCollider_ = std::make_unique<StageTransitionCollider>();
    transform_.scale={ 1.0f, 1.0f, 1.0f };
    transform_.rotate={ 0.0f, 0.0f, 0.0f };
    transform_.translate = stageData.transitionPoint;
    transitionCollider_->SetTransform(&transform_);
    transitionCollider_->SetOnTransitionTrigger([this]() { OnColliderHit(); });
    transitionCollider_->SetTypeID(100); // 適当な型IDを設定 TODO : enum 
    transitionCollider_->SetActive(false);

    Tako::CollisionManager::GetInstance()->AddCollider(transitionCollider_.get());
    Tako::CollisionManager::GetInstance()->SetCollisionMask(1, 100, true);

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

void StageClearFlow::NotifyClear()
{
    ClearEnter();
    // 仮
    OnColliderHit();
}

void StageClearFlow::ClearEnter()
{
    state_ = StageFlowState::StageClear;
    transitionCollider_->SetActive(true);
}

void StageClearFlow::FadeOutEnter()
{
    state_ = StageFlowState::FadeOut;
}

void StageClearFlow::FadeInEnter()
{
    state_ = StageFlowState::FadeIn;
}

void StageClearFlow::ClearUpdate(float deltaTime)
{
    // トランジションへの遷移はコライダーがイベントを受け取ったときなどの条件で行う想定
}

void StageClearFlow::FadeOutUpdate(float deltaTime)
{
    // 暗転終了時の処理
    if (Tako::Input::GetInstance()->TriggerKey(DIK_P))// デバッグ用にキー入力でフェードアウト完了をトリガー
    {
        if (onFadeOutComplete_) onFadeOutComplete_();
        state_ = StageFlowState::FadeIn;
    }
}

void StageClearFlow::FadeInUpdate(float deltaTime)
{
    //if (onFadeInComplete_) onFadeInComplete_();
    //state_ = StageFlowState::Playing; // フェードイン完了後は通常プレイ状態に戻す
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

void StageClearFlow::OnColliderHit()
{
    FadeOutEnter();
    Tako::Logger::Log("Transition triggered by collider hit!");
}
