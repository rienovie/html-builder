#include "UI_Windows.h"

std::map<std::string,bool> win::mWindowBools {
    {"demo",false},
    {"settings",false},
    {"test",false}
};

void win::mainLoop() {
    wMain();
    if(mWindowBools["demo"]) { ImGui::ShowDemoWindow(); }
    if(mWindowBools["settings"]) { wSettings(); }
    if(mWindowBools["test"]) { wTest(); }
}

void win::wMain() {
    ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());

    ImGui::Begin("Main",NULL,ImGuiWindowFlags_NoCollapse);
    if(ImGui::Button("Toggle Test Window")) { mWindowBools["test"] = !mWindowBools["test"]; }
    if(ImGui::Button("Toggle Demo Window")) { mWindowBools["demo"] = !mWindowBools["demo"]; }
    if(ImGui::Button("Toggle Settings Window")) { mWindowBools["settings"] = !mWindowBools["settings"]; }
    ImGui::End();
}

void win::wSettings() {
    ImGui::Begin("Settings",NULL);

    ImGui::SeparatorText("System");

    if(ImGui::Button("Default")) { UI::iFontSize = 24; };
    ImGui::SameLine();
    ImGui::PushFont(UI::font_bold);
    ImGui::SliderInt("Font Size",&UI::iFontSize,UI::limitFontSize.x,UI::limitFontSize.y);
    ImGui::PopFont();

    /* using this to write to the config file, will probably clean up later
    if(ImGui::Button("CLick")) {
        ImVec4* colors = ImGui::GetStyle().Colors;
        colors[ImGuiCol_Text]                   = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
        colors[ImGuiCol_TextDisabled]           = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
        colors[ImGuiCol_WindowBg]               = ImVec4(0.00f, 0.10f, 0.29f, 0.94f);
        colors[ImGuiCol_ChildBg]                = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        colors[ImGuiCol_PopupBg]                = ImVec4(0.00f, 0.05f, 0.14f, 0.97f);
        colors[ImGuiCol_Border]                 = ImVec4(1.00f, 1.00f, 1.00f, 0.23f);
        colors[ImGuiCol_BorderShadow]           = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        colors[ImGuiCol_FrameBg]                = ImVec4(0.36f, 0.31f, 1.00f, 0.20f);
        colors[ImGuiCol_FrameBgHovered]         = ImVec4(0.31f, 0.39f, 1.00f, 0.62f);
        colors[ImGuiCol_FrameBgActive]          = ImVec4(0.31f, 0.39f, 1.00f, 1.00f);
        colors[ImGuiCol_TitleBg]                = ImVec4(0.07f, 0.06f, 0.20f, 1.00f);
        colors[ImGuiCol_TitleBgActive]          = ImVec4(0.36f, 0.31f, 1.00f, 0.22f);
        colors[ImGuiCol_TitleBgCollapsed]       = ImVec4(0.00f, 0.00f, 0.00f, 0.50f);
        colors[ImGuiCol_MenuBarBg]              = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
        colors[ImGuiCol_ScrollbarBg]            = ImVec4(0.36f, 0.31f, 1.00f, 0.10f);
        colors[ImGuiCol_ScrollbarGrab]          = ImVec4(0.17f, 0.22f, 0.55f, 1.00f);
        colors[ImGuiCol_ScrollbarGrabHovered]   = ImVec4(0.22f, 0.28f, 0.71f, 1.00f);
        colors[ImGuiCol_ScrollbarGrabActive]    = ImVec4(0.31f, 0.39f, 1.00f, 1.00f);
        colors[ImGuiCol_CheckMark]              = ImVec4(0.31f, 0.39f, 1.00f, 1.00f);
        colors[ImGuiCol_SliderGrab]             = ImVec4(0.24f, 0.52f, 0.88f, 1.00f);
        colors[ImGuiCol_SliderGrabActive]       = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
        colors[ImGuiCol_Button]                 = ImVec4(0.26f, 0.59f, 0.98f, 0.40f);
        colors[ImGuiCol_ButtonHovered]          = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
        colors[ImGuiCol_ButtonActive]           = ImVec4(0.06f, 0.53f, 0.98f, 1.00f);
        colors[ImGuiCol_Header]                 = ImVec4(0.26f, 0.59f, 0.98f, 0.31f);
        colors[ImGuiCol_HeaderHovered]          = ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
        colors[ImGuiCol_HeaderActive]           = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
        colors[ImGuiCol_Separator]              = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
        colors[ImGuiCol_SeparatorHovered]       = ImVec4(0.10f, 0.40f, 0.75f, 0.78f);
        colors[ImGuiCol_SeparatorActive]        = ImVec4(0.10f, 0.40f, 0.75f, 1.00f);
        colors[ImGuiCol_ResizeGrip]             = ImVec4(0.26f, 0.59f, 0.98f, 0.20f);
        colors[ImGuiCol_ResizeGripHovered]      = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
        colors[ImGuiCol_ResizeGripActive]       = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
        colors[ImGuiCol_Tab]                    = ImVec4(0.18f, 0.35f, 0.58f, 0.86f);
        colors[ImGuiCol_TabHovered]             = ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
        colors[ImGuiCol_TabActive]              = ImVec4(0.20f, 0.41f, 0.68f, 1.00f);
        colors[ImGuiCol_TabUnfocused]           = ImVec4(0.07f, 0.10f, 0.15f, 0.97f);
        colors[ImGuiCol_TabUnfocusedActive]     = ImVec4(0.14f, 0.26f, 0.42f, 1.00f);
        colors[ImGuiCol_DockingPreview]         = ImVec4(0.26f, 0.59f, 0.98f, 0.70f);
        colors[ImGuiCol_DockingEmptyBg]         = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
        colors[ImGuiCol_PlotLines]              = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
        colors[ImGuiCol_PlotLinesHovered]       = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
        colors[ImGuiCol_PlotHistogram]          = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
        colors[ImGuiCol_PlotHistogramHovered]   = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
        colors[ImGuiCol_TableHeaderBg]          = ImVec4(0.19f, 0.19f, 0.20f, 1.00f);
        colors[ImGuiCol_TableBorderStrong]      = ImVec4(0.31f, 0.31f, 0.35f, 1.00f);
        colors[ImGuiCol_TableBorderLight]       = ImVec4(0.23f, 0.23f, 0.25f, 1.00f);
        colors[ImGuiCol_TableRowBg]             = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        colors[ImGuiCol_TableRowBgAlt]          = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
        colors[ImGuiCol_TextSelectedBg]         = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
        colors[ImGuiCol_DragDropTarget]         = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
        colors[ImGuiCol_NavHighlight]           = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
        colors[ImGuiCol_NavWindowingHighlight]  = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
        colors[ImGuiCol_NavWindowingDimBg]      = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
        colors[ImGuiCol_ModalWindowDimBg]       = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);

        auto mTheColVal = config::getAllThemeColorValues();
        for(int i = 0; i < ImGuiCol_COUNT;i++) {
            std::string sBuild = "~";
            sBuild.append(ImGui::GetStyleColorName(i));

            try {
                mTheColVal[sBuild]; //try
                config::update(config::theme,sBuild.c_str(),UI::getStringFromVec4(UI::uiStylePtr->Colors[i]));
            } catch (const std::out_of_range& e) {
                util::qPrint(sBuild,"prop not found!");
                continue;
            }
        }
    } */

    ImGui::SeparatorText("Theme");

    if (ImGui::BeginCombo("Current Theme",UI::vFoundThemes[UI::iCurrentTheme].c_str())){
        //for each theme
        for(int i = 0;i<UI::vFoundThemes.size();i++){
            const bool bSelected = (UI::iCurrentTheme == i);
            if(ImGui::Selectable(UI::vFoundThemes[i].c_str(),bSelected)) {
                UI::iCurrentTheme = i;
                //selection change is here
                config::update(config::system,"theme",UI::vFoundThemes[i]);
                UI::refreshTheme();
            }
            if( bSelected ) { ImGui::SetItemDefaultFocus(); }
        }
        ImGui::EndCombo();
    }

    if(ImGui::Button("Save Theme")) {

    }

    ImGui::Indent();
    swThemeOptions();
    swThemeColors();
    ImGui::Unindent();

    ImGui::End();
}

void win::wTest() {
    ImGui::Begin("Test Window",NULL);
    ImGui::PushFont(UI::font_light);
    ImGui::Text("I'm a test window!");
    ImGui::PopFont();
    ImGui::PushFont(UI::font_main);
    if(ImGui::Button("Close")) { mWindowBools["test"] = false; }
    ImGui::PopFont();
    ImGui::End();
}

void win::swCircleTessTT(bool show_samples) {
    if (show_samples)
        ImGui::SetNextWindowPos(ImGui::GetCursorScreenPos());
    if (show_samples && ImGui::BeginTooltip())
    {
        ImGui::TextUnformatted("(R = radius, N = number of segments)");
        ImGui::Spacing();
        ImDrawList* draw_list = ImGui::GetWindowDrawList();
        const float min_widget_width = ImGui::CalcTextSize("N: MMM\nR: MMM").x;
        for (int n = 0; n < 8; n++)
        {
            const float RAD_MIN = 5.0f;
            const float RAD_MAX = 70.0f;
            const float rad = RAD_MIN + (RAD_MAX - RAD_MIN) * (float)n / (8.0f - 1.0f);

            ImGui::BeginGroup();

            ImGui::Text("R: %.f\nN: %d", rad, draw_list->_CalcCircleAutoSegmentCount(rad));

            const float canvas_width = IM_MAX(min_widget_width, rad * 2.0f);
            const float offset_x     = floorf(canvas_width * 0.5f);
            const float offset_y     = floorf(RAD_MAX);

            const ImVec2 p1 = ImGui::GetCursorScreenPos();
            draw_list->AddCircle(ImVec2(p1.x + offset_x, p1.y + offset_y), rad, ImGui::GetColorU32(ImGuiCol_Text));
            ImGui::Dummy(ImVec2(canvas_width, RAD_MAX * 2));

            /*
             *       const ImVec2 p2 = ImGui::GetCursorScreenPos();
             *       draw_list->AddCircleFilled(ImVec2(p2.x + offset_x, p2.y + offset_y), rad, ImGui::GetColorU32(ImGuiCol_Text));
             *       ImGui::Dummy(ImVec2(canvas_width, RAD_MAX * 2));
             */

            ImGui::EndGroup();
            ImGui::SameLine();
        }
        ImGui::EndTooltip();
    }
}

void win::swThemeOptions() {
    ImGui::SeparatorText("Options");

    if(ImGui::SliderFloat("Frame Rounding",
                        &UI::uiStylePtr->FrameRounding,
                        UI::limitFrameRounding.x,
                        UI::limitFrameRounding.y,
                        "%.0f")) {
        //grab & popup = same as frame
        UI::uiStylePtr->GrabRounding = UI::uiStylePtr->FrameRounding;
        UI::uiStylePtr->PopupRounding = UI::uiStylePtr->FrameRounding;
        config::update(config::theme,"frameRounding",std::to_string(UI::uiStylePtr->FrameRounding));
    }

    if(ImGui::SliderFloat("Frame Border",
                        &UI::uiStylePtr->FrameBorderSize,
                        UI::limitFrameBorder.x,
                        UI::limitFrameBorder.y,
                        "%.0f")) {
        //popup same as frame
        UI::uiStylePtr->PopupBorderSize = UI::uiStylePtr->FrameBorderSize;
        config::update(config::theme,"frameBorder",std::to_string(UI::uiStylePtr->FrameBorderSize));
    }

    if(ImGui::SliderFloat2("Frame Padding",
                        (float*)&UI::uiStylePtr->FramePadding,
                        UI::limitFramePad.x,
                        UI::limitFramePad.y,
                        "%.0f")) {
        config::update(config::theme,"framePad",UI::getStringFromVec2(UI::uiStylePtr->FramePadding));
    }
    if(ImGui::SliderFloat2("Spacing",
                        (float*)&UI::uiStylePtr->ItemSpacing,
                        UI::limitItemSpacing.x,
                        UI::limitItemSpacing.y,
                        "%.0f")) {
        //inner same
        UI::uiStylePtr->ItemInnerSpacing = UI::uiStylePtr->ItemSpacing;
        config::update(config::theme,"itemSpacing",UI::getStringFromVec2(UI::uiStylePtr->ItemSpacing));
    }

    if(ImGui::SliderFloat("Separator Thickness",
                        &UI::uiStylePtr->SeparatorTextBorderSize,
                        UI::limitSeparatorThickness.x,
                        UI::limitSeparatorThickness.y,
                        "%.1f")) {
        config::update(config::theme,"separatorThickness",std::to_string(UI::uiStylePtr->SeparatorTextBorderSize));
    }

    if(ImGui::SliderFloat("Minimum Grab Size",
                        &UI::uiStylePtr->GrabMinSize,
                        UI::limitGrabMinSize.x,
                        UI::limitGrabMinSize.y,
                        "%.0f")) {
        config::update(config::theme,"grabMinSize",std::to_string(UI::uiStylePtr->GrabMinSize));
    }

    if(ImGui::SliderFloat("Tab Rounding",
                        &UI::uiStylePtr->TabRounding,
                        UI::limitTabRounding.x,
                        UI::limitTabRounding.y,
                        "%.0f")) {
        config::update(config::theme,"tabRounding",std::to_string(UI::uiStylePtr->TabRounding));
    }

    if(ImGui::SliderFloat("Scrollbar Size",
                        &UI::uiStylePtr->ScrollbarSize,
                        UI::limitScrollBarSize.x,
                        UI::limitScrollBarSize.y,
                        "%.0f")) {
        config::update(config::theme,"scrollBarSize",std::to_string(UI::uiStylePtr->ScrollbarSize));
    }

    if(ImGui::SliderFloat("Scrollbar Rounding",
                        &UI::uiStylePtr->ScrollbarRounding,
                        UI::limitScrollBarRounding.x,
                        UI::limitScrollBarRounding.y,
                        "%.0f")) {
        config::update(config::theme,"scrollBarRounding",std::to_string(UI::uiStylePtr->ScrollbarRounding));
    }

    if(ImGui::SliderFloat("Circle Tess",
                        &UI::uiStylePtr->CircleTessellationMaxError,
                        UI::limitCircleTess.x,
                        UI::limitCircleTess.y,
                        "%.2f",
                        ImGuiSliderFlags_AlwaysClamp)) {
        config::update(config::theme,"circleTess",std::to_string(UI::uiStylePtr->CircleTessellationMaxError));
    }

    swCircleTessTT(ImGui::IsItemActive());
}

void win::swThemeColors() {
    ImGui::SeparatorText("Colors");

    auto mColors = config::getAllThemeColorValues();
    int i = 0;
    for(auto item : mColors) {
        i = UI::getColorEnum(item.first);
        if(ImGui::ColorEdit4(ImGui::GetStyleColorName(i),(float*)&UI::uiStylePtr->Colors[i])) {
            config::update(config::theme,item.first.c_str(),UI::getStringFromVec4(UI::uiStylePtr->Colors[i]));
        }
    }

}

