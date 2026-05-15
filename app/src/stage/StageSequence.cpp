#include "StageSequence.h"

// 仮 
// 本番はstage::initの引数でファイルパス指定 読み込んだデータをもらってくるように
static std::vector <StageData> stageDataList = {
    // ステージデータの例
    { {0.0f, 2.0f, 3.0f} },
    { {5.0f, 2.0f, -2.0f} },
    { {-5.0f, 2.0f, 5.0f} },
};

void StageSequence::Initialize()
{
    clearFlow_.Initialize(stageDataList[0]);
    clearFlow_.SetOnFadeOutComplete([this]() { OnTransitionStage(); });


    const size_t numStages = 3; // ステージ数は適宜変更
    for (size_t i = 0; i < numStages; ++i)
    {
        auto stage = std::make_unique<Stage>();
        stage->Initialize(""); // ファイルパスは必要に応じて指定
        stages_.push_back(std::move(stage));
    }
}

void StageSequence::Update(float deltaTime)
{
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
}

void StageSequence::OnTransitionStage()
{
    ++currentIndex_;

    clearFlow_.Initialize(stageDataList[currentIndex_]);
}
