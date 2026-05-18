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
        if (std::holds_alternative<int*>(data))
        {
            ImGui::DragInt(name.c_str(), std::get<int*>(data));
        }
        else if (std::holds_alternative<float*>(data))
        {
            ImGui::DragFloat(name.c_str(), std::get<float*>(data));
        }
        else if (std::holds_alternative<bool*>(data))
        {
            ImGui::Checkbox(name.c_str(), std::get<bool*>(data));
        }
        else if (std::holds_alternative<std::string*>(data))
        {
            char buffer[256];
            strncpy_s(buffer, sizeof(buffer), std::get<std::string*>(data)->c_str(), _TRUNCATE);
            if (ImGui::InputText(name.c_str(), buffer, sizeof(buffer)))
            {
                *std::get<std::string*>(data) = buffer;
            }
        }
        else if (std::holds_alternative<Tako::Transform>(data))
        {
            Tako::Transform& transform = std::get<Tako::Transform>(data);
            ImGui::DragFloat3((name + " Scale").c_str(), &transform.scale.x, 0.01f);
            ImGui::DragFloat3((name + " Rotate").c_str(), &transform.rotate.x, 0.01f);
            ImGui::DragFloat3((name + " Translate").c_str(), &transform.translate.x, 0.01f);
        }
        else if (std::holds_alternative<Tako::Vector4*>(data))
        {
            ImGui::DragFloat4(name.c_str(), &std::get<Tako::Vector4*>(data)->x, 0.01f);
        }
        else if (std::holds_alternative<Tako::Vector3*>(data))
        {
            ImGui::DragFloat3(name.c_str(), &std::get<Tako::Vector3*>(data)->x, 0.01f);
        }
        else if (std::holds_alternative<Tako::Vector2*>(data))
        {
            ImGui::DragFloat2(name.c_str(), &std::get<Tako::Vector2*>(data)->x, 0.01f);
        }
    }

#endif // _DEBUG
}
