#include "DebugUIWrapper.h"
#include <imgui.h>
#include <debug/DebugRegisterer.h>


void DebugUIWrapper::Initialize()
{
    ImGui::GetStyle().IndentSpacing = 16.0f;

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
    for (auto& [id, entry] : entries_)
    {
        if (ImGui::TreeNode(entry->GetCategory().c_str()))
        {
            ImGui::Indent();
            entry->ImGui();
            ImGui::Unindent();

            ImGui::TreePop();
        }
    }
}
