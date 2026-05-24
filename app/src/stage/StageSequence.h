#pragma once

#include "stage.h"
#include "StageClearFlow.h"
#include <vector>
#include <filesystem>

using OnPlayerRespawnRequiredCallback  = std::function<void(const Tako::Transform&)>;

/// <summary>
/// 複数ステージの進行順序を管理するクラス
/// </summary>
class StageSequence
{
public:

    void Initialize(const std::string& jsonFilePath);
    void Update(float deltaTime);
    void Draw();

    void NotifyClear(); // ステージクリアを

    void DrawTransition();


    void SetOnStageChanged(OnPlayerRespawnRequiredCallback cb) { onStageChanged_ = cb; }


private:

    void OnTransitionStage();

    void LoadFromJson(const std::string& path);  // JSON読み込み & stages_再構築
    void CheckHotReload();
private:

    std::vector<std::unique_ptr<Stage>> stages_; 
    int32_t currentIndex_ = 0;
    StageClearFlow clearFlow_; // ステージクリアフローのインスタンス
    std::vector<StageData> stageDataList_; // ステージデータのリスト（JSONから読み込む）
    std::string jsonFilePath_;
    std::filesystem::file_time_type lastWriteTime_;


    OnPlayerRespawnRequiredCallback onStageChanged_; // ステージ変更時のコールバック
};