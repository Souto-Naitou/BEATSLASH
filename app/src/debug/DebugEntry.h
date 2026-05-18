#pragma once
#include <variant>
#include <string>
#include <Vector3.h>
#include <Vector4.h>
#include <Vector2.h>
#include <unordered_map>
#include <Transform.h>

class DebugEntry
{
public:
    DebugEntry(const std::string& id, const std::string category);
    ~DebugEntry();

    void ImGui();

    template <typename T>
    void RegisterParameter(const std::string& name, T* ptr);

    const std::string& GetCategory() const { return category_; }

private:
    using AvailableType = std::variant<int*, float*, bool*, std::string*, Tako::Transform, Tako::Vector4*, Tako::Vector3*, Tako::Vector2*>;
    std::unordered_map<std::string, AvailableType> parameters_;
    std::string category_;
};

template <typename T>
void DebugEntry::RegisterParameter(const std::string& name, T* ptr)
{
    parameters_.insert({name, ptr});
}
