#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <debug/DebugEntry.h>
#include <functional>

class DebugUIWrapper
{
public:
    static inline DebugUIWrapper* GetInstance()
    {
        static DebugUIWrapper instance;
        return &instance;
    }

    void Initialize();
    void Finalize();

    void RegisterEntry(const std::string& id, DebugEntry* pDebugEntry);
    void UnregisterEntry(DebugEntry* pDebugEntry);

    void ImGui();

    template <typename T>
    void HandleParameter(
        const std::string& id, 
        const std::string& name, 
        T* ptr, 
        std::function<void()> pFunc = nullptr);

    template <typename T>
    void HandleParameter(
        const std::string& id,
        const std::string& name,
        const T* ptr);

private:
    std::unordered_map<std::string, DebugEntry*> entries_;
};

template <typename T>
void DebugUIWrapper::HandleParameter(const std::string& id, const std::string& name, T* ptr, std::function<void()> pFunc)
{
    entries_.at(id)->RegisterParameter(name, ptr, std::move(pFunc));
}

template <typename T>
void DebugUIWrapper::HandleParameter(const std::string& id, const std::string& name, const T* ptr)
{
    entries_.at(id)->RegisterParameter(name, ptr);
}
