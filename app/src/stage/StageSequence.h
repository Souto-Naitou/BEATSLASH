#pragma once

#include "stage.h"
#include "StageClearFlow.h"

#include <vector>

/// <summary>
/// 複数ステージの進行順序を管理するクラス
/// </summary>
class StageSequence
{
public:

    void Initialize();
    void Update(float deltaTime);
    void Draw();

    void NotifyClear(); // ステージクリアを

private:

    void OnTransitionStage();

private:

    std::vector<std::unique_ptr<Stage>> stages_; 
    int32_t currentIndex_ = 0;
    StageClearFlow clearFlow_; // ステージクリアフローのインスタンス

};