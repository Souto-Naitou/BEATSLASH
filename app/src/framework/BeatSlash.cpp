#include "BeatSlash.h"

#include "Input.h"
#include "SceneManager.h"
#include <factory/SceneFactory.h>

#include <ozSound/audio/AudioSystem.h>
#include <ozSound/audio/SoundEngine.h>
#include <utility/DeltaTimeManager.h>
#include <TextureManager.h>

using namespace Tako;

void BeatSlash::Initialize()
{
    winApp_->SetWindowSize(1600, 900);

    winApp_->SetWindowTitle(L"ビート スラッシュ！");

    TakoFramework::Initialize();

    pDebugUIWrapper_ = DebugUIWrapper::GetInstance();
    pDebugUIWrapper_->Initialize();

    ozSound::AudioSystem::GetInstance()->Initialize();
        
    ozSound::SoundEngine::GetInstance()->Initialize();
    ozSound::SoundEngine::GetInstance()->LoadSoundData("resources/audio/SoundData.json");
    ozSound::SoundEngine::GetInstance()->LoadEventData("resources/audio/SoundEvents.json");

    // テクスチャの読み込み
	Tako::TextureManager::GetInstance()->LoadTexture("circle2.png");
	Tako::TextureManager::GetInstance()->LoadTexture("ring.png");
    Tako::TextureManager::GetInstance()->LoadTexture("notes_1.png");
    Tako::TextureManager::GetInstance()->LoadTexture("notes_2.png");

    DeltaTimeManager::GetInstance()->SetDeltaTime(DeltaTimeChannelReserved::Default, 1.0f / 60.0f);
    DeltaTimeManager::GetInstance()->SetDeltaTime(DeltaTimeChannelReserved::Game, 1.0f / 60.0f);

    // シーンの初期化
    sceneFactory_ = std::make_unique<SceneFactory>();
    SceneManager::GetInstance()->SetSceneFactory(sceneFactory_.get());
    SceneManager::GetInstance()->ChangeScene("title", 0.0f);
}

void BeatSlash::Finalize()
{
    TakoFramework::Finalize();
}

void BeatSlash::Update()
{
    // F11キーでフルスクリーン切り替え
    if (Input::GetInstance()->TriggerKey(DIK_F11))
    {
        ToggleFullScreen();
    }

    TakoFramework::Update();

    // 再生終了したボイスのエントリを解放する
    ozSound::SoundEngine::GetInstance()->CleanupStoppedVoices();
}

void BeatSlash::Draw()
{
    TakoFramework::Draw();
}