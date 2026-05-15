#include "StageSequence.h"
#include <ozSound/audio/JsonUtils/JsonUtils.h>

void StageSequence::Initialize(const std::string& jsonFilePath)
{
    jsonFilePath_ = jsonFilePath;
    LoadFromJson(jsonFilePath_);

    clearFlow_.Initialize(stageDataList_[0]);
    clearFlow_.SetOnFadeOutComplete([this]() { OnTransitionStage(); });
}

void StageSequence::Update(float deltaTime)
{
#ifdef _DEBUG
    CheckHotReload();
#endif
    clearFlow_.Update(deltaTime);
    if (currentIndex_ < stages_.size())
    {
        stages_[currentIndex_]->Update(deltaTime);
    }
}

void StageSequence::Draw()
{
    if (currentIndex_ < stages_.size())
    {
        stages_[currentIndex_]->Draw();
    }
}

void StageSequence::NotifyClear()
{
    clearFlow_.NotifyClear();
    stages_[currentIndex_]->OpenDoor();
}

void StageSequence::OnTransitionStage()
{
    ++currentIndex_;

    if (currentIndex_ < static_cast<int32_t>(stageDataList_.size()))
    {
        clearFlow_.Initialize(stageDataList_[currentIndex_]);
    }
    else
    {
        // 全ステージクリア → TODO: リザルト遷移
        currentIndex_ = 0;
    }
}

void StageSequence::LoadFromJson(const std::string& path)
{
    auto j = ozSound::LoadJson(path);
    if (j.is_discarded() || !j.contains("stages")) return;

    // コライダー解放 & stages_ クリア（ホットリロード時も安全に再構築）
    stages_.clear();
    stageDataList_.clear();

    for (auto& s : j["stages"])
    {
        StageData data;

        auto& tc = s["transitionCollider"];
        auto& tct = tc["translate"]; auto& tcr = tc["rotate"]; auto& tcs = tc["scale"];
        data.transitionTransform.translate = { tct["x"], tct["y"], tct["z"] };
        data.transitionTransform.rotate    = { tcr["x"], tcr["y"], tcr["z"] };
        data.transitionTransform.scale     = { tcs["x"], tcs["y"], tcs["z"] };

        auto& d = s["door"];
        auto& dt = d["translate"];
        auto& dr = d["rotate"];
        auto& ds = d["scale"];
        data.doorTransform.translate = { dt["x"], dt["y"], dt["z"] };
        data.doorTransform.rotate    = { dr["x"], dr["y"], dr["z"] };
        data.doorTransform.scale     = { ds["x"], ds["y"], ds["z"] };

        stageDataList_.push_back(data);

        auto stage = std::make_unique<Stage>();
        stage->Initialize(data);   // ← StageData を渡す
        stages_.push_back(std::move(stage));
    }

    // ホットリロード後も現在インデックスを維持（範囲外なら 0 にクランプ）
    if (currentIndex_ >= static_cast<int32_t>(stages_.size()))
        currentIndex_ = 0;

    lastWriteTime_ = std::filesystem::last_write_time(jsonFilePath_);
}

void StageSequence::CheckHotReload()
{
    auto t = std::filesystem::last_write_time(jsonFilePath_);
    if (t != lastWriteTime_)
    {
        LoadFromJson(jsonFilePath_);
        // clearFlow も現在ステージで再初期化
        clearFlow_.Initialize(stageDataList_[currentIndex_]);
    }
}