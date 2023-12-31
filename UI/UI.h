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

namespace UI {
    int initialize();
    void cleanUp();
    void mainLoop();
    void showMainUI();
    void showTestWindow();
    void showSettingsWindow();
    bool mainLoopCondition();
    void glfw_error_callback(int error, const char* description);
    void assignCurrentThemeValueByName(std::string name);
    void tick_sec();
    void refreshTheme();
    ImVec4 getVec4FromString(std::string sVec4Value);
    ImVec2 getVec2FromString(std::string sVec2Value);
    int getColorEnum(std::string sColorName);
    void setColorEnumMap();
}

