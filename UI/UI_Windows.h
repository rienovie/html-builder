#pragma once

//needed for the circle Tess Setting
#define IM_MAX(A, B)            (((A) >= (B)) ? (A) : (B))

#include "../sys/util.h"
#include "ImGui/imgui.h"
#include "ImGui/imgui_stdlib.h"
#include "../sys/config.h"
#include "UI.h"
#include <map>
#include <string>
#include <math.h>
#include <limits.h>
#include "../sys/html.h"
#include <filesystem>

class win {
public:
    static void mainLoop();
private:
    static std::map<std::string,bool> mWindowBools;

    static void wMainMenu();
    static void wSettings();
    static void wTest();
    static void wHierarchy();
    static void wFileRoot(html::file* filePTR);
    static void wFileBrowser();
    static void swCircleTessTT(bool show_samples);
    static void swThemeOptions();
    static void swThemeColors();
    static int themeNameCallback(ImGuiInputTextCallbackData* data);
    static void BasicToolTip(const char* toolTipText);
    static void hierarchyPopulate(html::element* element);
};
