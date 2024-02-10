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

    static void
        wMainMenu(),
        wSettings(),
        wTest(),
        wHierarchy(),
        wFileRoot(html::file* filePTR),
        wFileBrowser(),
        swCircleTessTT(bool show_samples),
        swThemeOptions(),
        swThemeColors(),
        BasicToolTip(const char* toolTipText),
        hierarchyPopulate(html::element* element),
        wSelectedElement();

    static int themeNameCallback(ImGuiInputTextCallbackData* data);
};
