#pragma once
#include <memory>

#include "StageTransitionCollider.h"

class StageClearFlow
{
public:

    enum class StageFlowState
    {
        Playing,// ユーザーが戦闘中？
        StageClear,// クリア済み
        Transitioning,// ステージ遷移中
    };

    void Initialize();

    void Update(float deltaTime);


    // ステージクリアした時に呼び出す
    void NotifyClear();

private:

    void ClearEnter();
    void TransitionEnter();

    void ClearUpdate(float deltaTime);
    void TransitionUpdate(float deltaTime);

    void ClearExit();
    void TransitionExit();

    // コライダーに渡すコールバック関数
    void OnColliderHit(/* 引数はコライダーのイベントに合わせて適宜追加 */);

private:

    StageFlowState state_ = StageFlowState::Playing;

    // 遷移用イベントコライダー
    std::unique_ptr<StageTransitionCollider> transitionCollider_;
};