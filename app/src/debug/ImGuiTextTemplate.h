#pragma once

#ifdef _DEBUG
#include <imgui.h>
#endif // _DEBUG

namespace ImGuiTemplate
{
    inline void TextBoolean(const char* label, bool value)
    {
#ifdef _DEBUG
        ImGui::Text("%s: ", label);
        ImGui::SameLine();
        ImGui::TextColored(value ? ImVec4(0.0f, 1.0f, 0.0f, 1.0f) : ImVec4(1.0f, 0.0f, 0.0f, 1.0f), value ? "True" : "False");
#endif // _DEBUG
    }
}