#pragma once

#include "ImGui/imgui.h"
#include "ImGui/imgui_stdlib.h"
#include "ImGui/imconfig.h"
#include "ImGui/backends/imgui_impl_glfw.h"
#include "ImGui/backends/imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>
#include "../sys/util.h"
#include <fstream>
#include <map>
#include <filesystem>
#include <vector>
#include "../sys/config.h"
#include "UI_Windows.h"

void glfw_error_callback(int error, const char* description);

class UI {
public:
    static int
        iFontSize,
        iCurrentTheme;
    static ImVec2
        limitFontSize,
        limitScrollBarSize,
        limitScrollBarRounding,
        limitTabRounding,
        limitFrameBorder,
        limitFrameRounding,
        limitFramePad,
        limitItemSpacing,
        limitSeparatorThickness,
        limitCircleTess,
        limitGrabMinSize;
    static std::vector<std::string> vFoundThemes;
    static ImGuiStyle* uiStylePtr;
    static ImFont *font_main, *font_bold, *font_light;

    static bool mainLoopCondition();
    static void mainLoop();
    static void tick_sec();
    static void assignCurrentThemeValueByName(std::string sName);
    static void refreshTheme();
    static int getColorEnum(std::string sColorName);
    static ImVec4 getVec4FromString(std::string sVec4Value);
    static ImVec2 getVec2FromString(std::string sVec2Value);
    static std::string getStringFromVec2(ImVec2 vec2Value);
    static std::string getStringFromVec4(ImVec4 vec4Value);

    UI();
    ~UI();

private:
    static int
        iUserWindowWidth,
        iUserWindowHeight,
        iLastStoredFontSize;

    static ImVec4 clearColor;
    static GLFWwindow *mainWindow;
    static std::map<std::string,int> mColorEnum;

    static void setColorEnumMap();

};

