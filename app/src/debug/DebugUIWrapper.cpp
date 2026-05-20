#include "DebugUIWrapper.h"
#include <debug/DebugRegisterer.h>
#include <math/HSV.h>
#include "utility/strutl.h"

#ifdef _DEBUG
#include <imgui.h>
#endif // _DEBUG


void DebugUIWrapper::Initialize()
{
#ifdef _DEBUG
    ImGui::GetStyle().IndentSpacing = 16.0f;
#endif // _DEBUG

    DebugRegister("Debug Entries", &DebugUIWrapper::ImGui, this);
}

void DebugUIWrapper::Finalize()
{
    DebugUnregister("Debug Entries");
}

void DebugUIWrapper::RegisterEntry(const std::string& id, DebugEntry* pDebugEntry)
{
    entries_[id] = pDebugEntry;
}

void DebugUIWrapper::UnregisterEntry(DebugEntry* pDebugEntry)
{
    for (auto it = entries_.begin(); it != entries_.end(); ++it)
    {
        if (it->second == pDebugEntry)
        {
            entries_.erase(it);
            break;
        }
    }
}

void DebugUIWrapper::ImGui()
{
#ifdef _DEBUG

    for (auto& [id, entry] : entries_)
    {
        uint32_t hash = utl::string::to_hash(entry->GetCategory());
        HSV hsv = { static_cast<float>(hash % 360) / 360.0f, 0.65f, 0.85f };

        ImVec4 col = {};
        ImGui::ColorConvertHSVtoRGB(hsv.h(), hsv.s(), hsv.v(), col.x, col.y, col.z);
        col.w = 1.0f;

        ImGui::PushStyleColor(ImGuiCol_Text, col);
        bool isOpen = ImGui::TreeNode(entry->GetCategory().c_str());
        if (isOpen)
        {
            ImGui::Indent();
            entry->ImGui();
            ImGui::Unindent();

            ImGui::TreePop();
        }
        ImGui::PopStyleColor();
    }

#endif // _DEBUG
}
