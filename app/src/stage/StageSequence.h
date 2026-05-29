#pragma once

#include "stage.h"
#include "StageClearFlow.h"
#include <vector>
#include <filesystem>
#include <functional>

using OnPlayerRespawnRequiredCallback  = std::function<void(const Tako::Transform&)>;
using OnDoorOpenedCallback = std::function<void(const Tako::Transform&)>;

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


    void SetOnStageChanged(OnPlayerRespawnRequiredCallback cb) { onStageChanged_ = std::move(cb); }
    void SetOnDoorOpened(OnDoorOpenedCallback cb) { onDoorOpened_ = std::move(cb); }
    void SetOnDoorOpenFinished(std::function<void()> cb) { onDoorOpenFinished_ = std::move(cb); }

    // カメラ補間完了後に呼ぶ
    void OpenCurrentDoor();


private:

    void OnTransitionStage();

    void LoadFromJson(const std::string& path);  // JSON読み込み & stages_再構築
    void CheckHotReload();
private:

    std::vector<std::unique_ptr<Stage>> stages_;
    int32_t currentIndex_ = 0;
    StageClearFlow clearFlow_;
    std::vector<StageData> stageDataList_;
    std::string jsonFilePath_;
    std::filesystem::file_time_type lastWriteTime_;
    bool hasClearNotified_ = false; // ステージ遷移まで再通知を防ぐ


    OnPlayerRespawnRequiredCallback onStageChanged_;
    OnDoorOpenedCallback onDoorOpened_;
    std::function<void()> onDoorOpenFinished_;
};