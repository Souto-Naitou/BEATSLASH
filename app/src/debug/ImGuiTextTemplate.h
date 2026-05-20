#pragma once

#include <imgui.h>

namespace ImGuiTemplate
{
    inline void TextBoolean(const char* label, bool value)
    {
        ImGui::Text("%s: ", label);
        ImGui::SameLine();
        ImGui::TextColored(value ? ImVec4(0.0f, 1.0f, 0.0f, 1.0f) : ImVec4(1.0f, 0.0f, 0.0f, 1.0f), value ? "True" : "False");
    }
}