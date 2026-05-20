#pragma once
#include <variant>
#include <string>
#include <Vector3.h>
#include <Vector4.h>
#include <Vector2.h>
#include <unordered_map>
#include <Transform.h>
#include <functional>

class DebugEntry
{
public:
    using AvailableType = std::variant<
        int*,
        float*,
        bool*,
        std::string*,
        Tako::Transform*,
        Tako::Vector4*,
        Tako::Vector3*,
        Tako::Vector2*
    >;

    using ConstAvailableType = std::variant<
        const int*,
        const float*,
        const bool*,
        const std::string*,
        const Tako::Transform*,
        const Tako::Vector4*,
        const Tako::Vector3*,
        const Tako::Vector2*
    >;

    struct ParameterData
    {
        AvailableType ptr;
        std::function<void()> onChange;
    };

    DebugEntry(const std::string& id, const std::string category);
    ~DebugEntry();

    void ImGui();

    template <typename T>
    void RegisterParameter(const std::string& name, T* ptr, std::function<void()> pFunc);

    template <typename T>
    void RegisterParameter(const std::string& name, const T* ptr);


    const std::string& GetCategory() const { return category_; }

private:
    std::unordered_map<std::string, ParameterData> parameters_;
    std::unordered_map<std::string, ConstAvailableType> parametersConstant_;
    std::string category_;
};

template <typename T>
void DebugEntry::RegisterParameter(const std::string& name, T* ptr, std::function<void()> pFunc)
{
    parameters_.insert({ name, {ptr, pFunc} });
}

template <typename T>
void DebugEntry::RegisterParameter(const std::string& name, const T* ptr)
{
    parametersConstant_.insert({ name, ptr });
}
