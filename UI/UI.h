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

namespace UI {
    int initialize();
    void cleanUp();
    void mainLoop();
    void showMainUI();
    void showTestWindow();
    void showSettingsWindow();
    bool mainLoopCondition();
    void glfw_error_callback(int error, const char* description);
    void setThemeByFile(const char* file);
    void saveThemeToFile(const char* themeDir, const char* name);
}

