#include "SceneFactory.h"

#include <scene/GameScene.h>
#include <scene/TitleScene.h>

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

    #ifdef _DEBUG
    else if (sceneName == "test")
    {
        return std::make_unique<TestScene>();
    }
    DebugUIManager::GetInstance()->AddLog("Unknown scene name: " + sceneName, DebugUIManager::LogType::Error);
    #endif

    return nullptr;
}