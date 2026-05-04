#include "RhythmActionGame.h"

#include "Input.h"
#include "SceneManager.h"
#include <factory/SceneFactory.h>

using namespace Tako;

void RhythmActionGame::Initialize()
{
  winApp_->SetWindowSize(1920, 1080);

  winApp_->SetWindowTitle(L"RhythmActionGame");

  TakoFramework::Initialize();

  // シーンの初期化
  sceneFactory_ = std::make_unique<SceneFactory>();
  SceneManager::GetInstance()->SetSceneFactory(sceneFactory_.get());
  SceneManager::GetInstance()->ChangeScene("sample", 0.0f);
}

void RhythmActionGame::Finalize()
{
  TakoFramework::Finalize();
}

void RhythmActionGame::Update()
{
  // F11キーでフルスクリーン切り替え
  if (Input::GetInstance()->TriggerKey(DIK_F11)) {
    ToggleFullScreen();
  }
  TakoFramework::Update();
}

void RhythmActionGame::Draw()
{
  TakoFramework::Draw();
}