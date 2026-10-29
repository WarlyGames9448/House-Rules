#pragma once

#include "Fuze.h"

#include <imgui.h>
#include <imgui_internal.h>

namespace Fuze {

class EditorPanel {
  public:
    EditorPanel() = default;
    explicit EditorPanel(std::string name): m_Name(std::move(name)) {
    }

    virtual ~EditorPanel() = default;

    virtual void OnImGuiRenderer() = 0;

    const std::string& GetName() const {
        return m_Name;
    }

  private:
    std::string m_Name;
};

namespace UI {
struct PanelOptions {
    ImGuiWindowFlags Flags = ImGuiWindowFlags_None;

    // nullptr = use the global theme padding.
    // Set to (0, 0) for panels such as the Viewport.
    const ImVec2* Padding = nullptr;

    // Optional visibility control.
    bool* Open = nullptr;
};

inline bool BeginPanel(const char* title, const PanelOptions& options = {}) {
    if (options.Padding != nullptr) {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, *options.Padding);
    }

    const bool visible = ImGui::Begin(title, options.Open, options.Flags);

    if (options.Padding != nullptr) {
        ImGui::PopStyleVar();
    }

    // IMPORTANT:
    // The caller must always call EndPanel(),
    // even when BeginPanel() returns false.
    return visible;
}

inline void EndPanel() {
    ImGui::End();
}

// Draws the teal selection indicator over the last ImGui item.
inline void DrawSelectionIndicator(bool selected, float width = 2.0f, float verticalInset = 2.0f) {
    if (!selected) return;

    const ImVec2 min = ImGui::GetItemRectMin();
    const ImVec2 max = ImGui::GetItemRectMax();

    // Ignore items without a valid visible area.
    if (max.x <= min.x || max.y <= min.y) return;

    const float inset = ImMin(verticalInset, (max.y - min.y) * 0.5f);

    ImGui::GetWindowDrawList()->AddRectFilled(
        ImVec2(min.x, min.y + inset), ImVec2(min.x + width, max.y - inset), ImGui::GetColorU32(ImGuiCol_CheckMark));
}
}
}
