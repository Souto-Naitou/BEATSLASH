#include "StageSequence.h"
#include <ozSound/audio/JsonUtils/JsonUtils.h>

#include <Input.h>

void StageSequence::Initialize(const std::string& jsonFilePath)
{
    jsonFilePath_ = jsonFilePath;
    LoadFromJson(jsonFilePath_);
    hasClearNotified_ = false;

    clearFlow_.Initialize(stageDataList_[0]);
    clearFlow_.SetOnFadeOutComplete([this]() { OnTransitionStage(); });
}

void StageSequence::Update(float deltaTime)
{
#ifdef _DEBUG
    CheckHotReload();

    if (Tako::Input::GetInstance()->TriggerKey(DIK_RETURN))
    {
        NotifyClear();
        hasClearNotified_ = false;
    }
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
    if (hasClearNotified_) return;
    hasClearNotified_ = true;

    clearFlow_.NotifyClear();
    // OpenDoor はカメラ補間完了後に OpenCurrentDoor() で呼ぶ
    if (onDoorOpened_)
        onDoorOpened_(stageDataList_[currentIndex_].doorTransform);
}

void StageSequence::OpenCurrentDoor()
{
    stages_[currentIndex_]->SetOnDoorOpenFinished([this]()
                                                  {
                                                      clearFlow_.ActivateTransitionCollider();
                                                      if (onDoorOpenFinished_) onDoorOpenFinished_();
                                                  });
    stages_[currentIndex_]->OpenDoor();
}

void StageSequence::DrawTransition()
{
    clearFlow_.Draw();
}

void StageSequence::OnTransitionStage()
{
    hasClearNotified_ = false;  // 次のステージで再度クリア通知を受け付ける
    int32_t preIndex = currentIndex_++;

    if (currentIndex_ < static_cast<int32_t>(stageDataList_.size()))
    {
        clearFlow_.Initialize(stageDataList_[currentIndex_]);
        stages_[currentIndex_]->CollisionActive(true); // 現在ステージのコライダーを有効
        for (size_t i = 0; i < stageDataList_.size(); ++i)
        {
            if (i != currentIndex_)
                stages_[preIndex]->CollisionActive(false); // 前のステージのコライダーを無効

        }
        if (onStageChanged_)
        {
            onStageChanged_(stageDataList_[currentIndex_].playerStartTransform);
        }
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

        for (auto& f : s["floors"])
        {
            Tako::Transform floorTf;
            auto& ft = f["translate"]; auto& fr = f["rotate"]; auto& fs = f["scale"];
            floorTf.translate = { ft["x"], ft["y"], ft["z"] };
            floorTf.rotate    = { fr["x"], fr["y"], fr["z"] };
            floorTf.scale     = { fs["x"], fs["y"], fs["z"] };
            data.floorTransform.push_back(floorTf);
        }

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

        auto& sp = s["spawn"];
        auto& spt = sp["translate"];
        auto& spr = sp["rotate"];
        data.playerStartTransform.translate = { spt["x"], spt["y"], spt["z"] };
        data.playerStartTransform.rotate    = { spr["x"], spr["y"], spr["z"] };

        stageDataList_.push_back(data);

        auto stage = std::make_unique<Stage>();
        stage->Initialize(data);   // ← StageData を渡す
        stage->CollisionActive(false); // 最初はコライダー無効
        stages_.push_back(std::move(stage));

    }


    // ホットリロード後も現在インデックスを維持（範囲外なら 0 にクランプ）
    if (currentIndex_ >= static_cast<int32_t>(stages_.size()))
        currentIndex_ = 0;

    lastWriteTime_ = std::filesystem::last_write_time(jsonFilePath_);

    stages_[currentIndex_]->CollisionActive(true); // 現在ステージのコライダーを有効
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
