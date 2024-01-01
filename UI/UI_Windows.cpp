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

    if (ImGui::BeginCombo("Theme",UI::vFoundThemes[UI::iCurrentTheme].c_str())){
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

    if(ImGui::Button("Default")) { UI::iFontSize = 24; };
    ImGui::SameLine();
    ImGui::PushFont(UI::font_bold);
    ImGui::SliderInt("Font Size",&UI::iFontSize,UI::limitFontSize.x,UI::limitFontSize.y);
    ImGui::PopFont();
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
