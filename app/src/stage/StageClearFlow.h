#pragma once
#include <memory>

#include "StageTransitionCollider.h"
#include "StageData.h"

#include <transition/FadeTransition.h>

class StageClearFlow
{
public:

    ~StageClearFlow();

    enum class StageFlowState
    {
        Playing,// ユーザーが戦闘中？
        StageClear,// クリア済み
        FadeOut,        // 暗転中
        FadeIn,         // 明転中
    };

    void Initialize(const StageData& stageData);

    void Update(float deltaTime);

    void Draw();

    // ステージクリアした時に呼び出す
    void NotifyClear();
    void ActivateTransitionCollider();
    void SetOnFadeOutComplete(std::function<void()> callback) { onFadeOutComplete_ = callback; }

private:

    void ClearEnter();
    void FadeOutEnter();
    void FadeInEnter();

    void ClearUpdate(float deltaTime);
    void FadeOutUpdate(float deltaTime);
    void FadeInUpdate(float deltaTime);

    void ClearExit();
    void FadeOutExit();
    void FadeInExit();


private:

    StageFlowState state_ = StageFlowState::Playing;

    // 遷移用イベントコライダー
    std::unique_ptr<StageTransitionCollider> transitionCollider_;
    Tako::Transform transform_;

    // 遷移完了時のコールバック関数
    std::function<void()> onFadeOutComplete_;
    std::function<void()> onFadeInComplete_;

    std::unique_ptr<Tako::FadeTransition> fade_;
    float fadeDuration_ = 1.0f; // フェードの時間（秒）
};