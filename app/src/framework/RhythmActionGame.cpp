#include "RhythmActionGame.h"

#include "Input.h"
#include "SceneManager.h"
#include <factory/SceneFactory.h>

#include <ozSound/audio/AudioSystem.h>
#include <ozSound/audio/SoundEngine.h>

using namespace Tako;

void RhythmActionGame::Initialize()
{
    winApp_->SetWindowSize(1920, 1080);

    winApp_->SetWindowTitle(L"RhythmActionGame");

    TakoFramework::Initialize();

    ozSound::AudioSystem::GetInstance()->Initialize();
        
    ozSound::SoundEngine::GetInstance()->Initialize();
    ozSound::SoundEngine::GetInstance()->LoadSoundData("resources/audio/SoundData.json");
    ozSound::SoundEngine::GetInstance()->LoadEventData("resources/audio/SoundEvents.json");

    // シーンの初期化
    sceneFactory_ = std::make_unique<SceneFactory>();
    SceneManager::GetInstance()->SetSceneFactory(sceneFactory_.get());
    SceneManager::GetInstance()->ChangeScene("game", 0.0f);
}

void RhythmActionGame::Finalize()
{
    TakoFramework::Finalize();
}

void RhythmActionGame::Update()
{
    // F11キーでフルスクリーン切り替え
    if (Input::GetInstance()->TriggerKey(DIK_F11))
    {
        ToggleFullScreen();
    }

    if (Input::GetInstance()->TriggerKey(DIK_SPACE))
    {
        // サウンドイベント "PlaySampleSE" を再生
        ozSound::SoundEngine::GetInstance()->PostEvent("play");
    }
    if (Input::GetInstance()->TriggerKey(DIK_S))
    {
        ozSound::SoundEngine::GetInstance()->StopAll();
    }

    TakoFramework::Update();
}

void RhythmActionGame::Draw()
{
    TakoFramework::Draw();
}