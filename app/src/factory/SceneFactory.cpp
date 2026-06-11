#include "SceneFactory.h"

#include <scene/GameScene.h>
#include <scene/TitleScene.h>
#include <scene/GameoverScene.h>

#ifdef _DEBUG
#include "DebugUIManager.h"
#include <test/TestScene.h>
#endif

using namespace Tako;

std::unique_ptr<Tako::BaseScene> SceneFactory::CreateScene(const std::string& sceneName)
{
    if (sceneName == "game")
    {
        return std::make_unique<GameScene>();
    }
    else if (sceneName == "title")
    {
        return std::make_unique<TitleScene>();
    }
    else if (sceneName == "gameover")
    {
        return std::make_unique<GameoverScene>();
    }

    #ifdef _DEBUG
    else if (sceneName == "test")
    {
        return std::make_unique<TestScene>();
    }
    DebugUIManager::GetInstance()->AddLog("Unknown scene name: " + sceneName, DebugUIManager::LogType::Error);
    #endif

    return nullptr;
}