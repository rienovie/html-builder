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

class win {
public:
    static void mainLoop();
private:
    static std::map<std::string,bool> mWindowBools;

    static void wMain();
    static void wSettings();
    static void wTest();
    static void swCircleTessTT(bool show_samples);
    static void swThemeOptions();
    static void swThemeColors();
};
