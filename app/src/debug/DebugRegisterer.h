#pragma once
#include <string>

#ifdef _DEBUG

    // デバッグビルドでのみ DebugUIManager をインクルード
    #include <DebugUIManager.h>
    #include <functional>
    #include <source_location>
    #include "DebugUIWrapper.h"

    // デバッグUIに情報を登録するマクロ 
    template<typename T1, typename T2>
    inline constexpr void DebugRegister(const std::string& name, T1 pFunc, T2 instance) { Tako::DebugUIManager::GetInstance()->RegisterGameObject(name, std::bind(pFunc, instance)); }
    inline void DebugUnregister(const std::string& name) { Tako::DebugUIManager::GetInstance()->UnregisterGameObject(name); }

    template<typename T1, typename T2>
    inline void RegisterCustomGui(const std::string& name, T1 pFunc, T2 instance)
    {
        DebugUIWrapper::GetInstance()->HandleCustomGuiFunction(std::source_location::current().file_name(), name, std::bind(pFunc, instance));
    }

#else

    template <typename T1, typename T2>
    inline constexpr auto DebugRegister(std::string, T1, T2) {}
    inline constexpr void DebugUnregister(std::string) {}

#endif // _DEBUG
