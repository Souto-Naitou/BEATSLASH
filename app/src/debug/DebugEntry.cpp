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
        if (std::holds_alternative<int*>(data.ptr))
        {
            if (ImGui::DragInt(name.c_str(), std::get<int*>(data.ptr)))
            {
                if (data.onChange)
                {
                    data.onChange();
                }
            }
        }
        else if (std::holds_alternative<float*>(data.ptr))
        {
            if (ImGui::DragFloat(name.c_str(), std::get<float*>(data.ptr)))
            {
                if (data.onChange)
                {
                    data.onChange();
                }
            }
        }
        else if (std::holds_alternative<bool*>(data.ptr))
        {
            if (ImGui::Checkbox(name.c_str(), std::get<bool*>(data.ptr)))
            {
                if (data.onChange)
                {
                    data.onChange();
                }
            }
        }
        else if (std::holds_alternative<std::string*>(data.ptr))
        {
            char buffer[256];
            strncpy_s(buffer, sizeof(buffer), std::get<std::string*>(data.ptr)->c_str(), _TRUNCATE);
            if (ImGui::InputText(name.c_str(), buffer, sizeof(buffer)))
            {
                *std::get<std::string*>(data.ptr) = buffer;
                if (data.onChange)
                {
                    data.onChange();
                }
            }
        }
        else if (std::holds_alternative<Tako::Transform>(data.ptr))
        {
            bool isChanged = false;
            Tako::Transform& transform = std::get<Tako::Transform>(data.ptr);
            isChanged |= ImGui::DragFloat3((name + " Scale").c_str(), &transform.scale.x, 0.01f);
            isChanged |= ImGui::DragFloat3((name + " Rotate").c_str(), &transform.rotate.x, 0.01f);
            isChanged |= ImGui::DragFloat3((name + " Translate").c_str(), &transform.translate.x, 0.01f);
            if (isChanged && data.onChange)
            {
                data.onChange();
            }
        }
        else if (std::holds_alternative<Tako::Vector4*>(data.ptr))
        {
            if (ImGui::DragFloat4(name.c_str(), &std::get<Tako::Vector4*>(data.ptr)->x, 0.01f))
            {
                if (data.onChange)
                {
                    data.onChange();
                }
            }
        }
        else if (std::holds_alternative<Tako::Vector3*>(data.ptr))
        {
            if (ImGui::DragFloat3(name.c_str(), &std::get<Tako::Vector3*>(data.ptr)->x, 0.01f))
            {
                if (data.onChange)
                {
                    data.onChange();
                }
            }
        }
        else if (std::holds_alternative<Tako::Vector2*>(data.ptr))
        {
            if (ImGui::DragFloat2(name.c_str(), &std::get<Tako::Vector2*>(data.ptr)->x, 0.01f))
            {
                if (data.onChange)
                {
                    data.onChange();
                }
            }
        }
    }

#endif // _DEBUG
}
