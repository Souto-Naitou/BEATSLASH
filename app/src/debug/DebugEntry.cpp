#include "DebugEntry.h"

#include <debug/DebugUIWrapper.h>
#include <imgui.h>

DebugEntry::DebugEntry(const std::string& id, const std::string category)
{
    DebugUIWrapper::GetInstance()->RegisterEntry(id, this);
    category_ = category;
}

DebugEntry::~DebugEntry()
{
    DebugUIWrapper::GetInstance()->UnregisterEntry(this);
}

void DebugEntry::ImGui()
{
#ifdef _DEBUG

    for (auto& [name, data] : parameters_)
    {
        std::visit([&](auto&& arg) {
            using T = std::decay_t<decltype(arg)>;
            if constexpr (std::is_same_v<T, int*>)
            {
                if (ImGui::DragInt(name.c_str(), arg))
                {
                    if (data.onChange)
                    {
                        data.onChange();
                    }
                }
            }
            else if constexpr (std::is_same_v<T, float*>)
            {
                if (ImGui::DragFloat(name.c_str(), arg))
                {
                    if (data.onChange)
                    {
                        data.onChange();
                    }
                }
            }
            else if constexpr (std::is_same_v<T, bool*>)
            {
                if (ImGui::Checkbox(name.c_str(), arg))
                {
                    if (data.onChange)
                    {
                        data.onChange();
                    }
                }
            }
            else if constexpr (std::is_same_v<T, std::string*>)
            {
                char buffer[256];
                strncpy_s(buffer, sizeof(buffer), arg->c_str(), _TRUNCATE);
                if (ImGui::InputText(name.c_str(), buffer, sizeof(buffer)))
                {
                    *arg = buffer;
                    if (data.onChange)
                    {
                        data.onChange();
                    }
                }
            }
            else if constexpr (std::is_same_v<T, Tako::Transform*>)
            {
                bool isChanged = false;
                isChanged |= ImGui::DragFloat3((name + " Scale").c_str(), &arg->scale.x, 0.01f);
                isChanged |= ImGui::DragFloat3((name + " Rotate").c_str(), &arg->rotate.x, 0.01f);
                isChanged |= ImGui::DragFloat3((name + " Translate").c_str(), &arg->translate.x, 0.01f);
                if (isChanged && data.onChange)
                {
                    data.onChange();
                }
            }
            else if constexpr (std::is_same_v<T, Tako::Vector4*>)
            {
                if (ImGui::DragFloat4(name.c_str(), &arg->x, 0.01f))
                {
                    if (data.onChange)
                    {
                        data.onChange();
                    }
                }
            }
            else if constexpr (std::is_same_v<T, Tako::Vector3*>)
            {
                if (ImGui::DragFloat3(name.c_str(), &arg->x, 0.01f))
                {
                    if (data.onChange)
                    {
                        data.onChange();
                    }
                }
            }
            else if constexpr (std::is_same_v<T, Tako::Vector2*>)
            {
                if (ImGui::DragFloat2(name.c_str(), &arg->x, 0.01f))
                {
                    if (data.onChange)
                    {
                        data.onChange();
                    }
                }
            }
        }, data.ptr);
    }

    /// 定数パラメータの表示
    /// （定数パラメータは値の変更はできないが、値の確認はできるようにする）

    for (auto& [name, data] : parametersConstant_)
    {
        std::visit([&](auto&& arg) {
            using T = std::decay_t<decltype(arg)>;
            if constexpr (std::is_same_v<T, const int*>)
            {
                ImGui::Text("%s: %d", name.c_str(), *arg);
            }
            else if constexpr (std::is_same_v<T, const float*>)
            {
                ImGui::Text("%s: %.2f", name.c_str(), *arg);
            }
            else if constexpr (std::is_same_v<T, const bool*>)
            {
                ImGui::Text("%s: %s", name.c_str(), *arg ? "True" : "False");
            }
            else if constexpr (std::is_same_v<T, const std::string*>)
            {
                ImGui::Text("%s: %s", name.c_str(), arg->c_str());
            }
            else if constexpr (std::is_same_v<T, const Tako::Transform*>)
            {
                ImGui::Text("%s Scale: (%.2f, %.2f, %.2f)", name.c_str(), arg->scale.x, arg->scale.y, arg->scale.z);
                ImGui::Text("%s Rotate: (%.2f, %.2f, %.2f)", name.c_str(), arg->rotate.x, arg->rotate.y, arg->rotate.z);
                ImGui::Text("%s Translate: (%.2f, %.2f, %.2f)", name.c_str(), arg->translate.x, arg->translate.y, arg->translate.z);
            }
            else if constexpr (std::is_same_v<T, const Tako::Vector4*>)
            {
                ImGui::Text("%s: (%.2f, %.2f, %.2f, %.2f)", name.c_str(), arg->x, arg->y, arg->z, arg->w);
            }
            else if constexpr (std::is_same_v<T, const Tako::Vector3*>)
            {
                ImGui::Text("%s: (%.2f, %.2f, %.2f)", name.c_str(), arg->x, arg->y, arg->z);
            }
            else if constexpr (std::is_same_v<T, const Tako::Vector2*>)
            {
                ImGui::Text("%s: (%.2f, %.2f)", name.c_str(), arg->x, arg->y);
            }
        }, data);
    }

#endif // _DEBUG
}
