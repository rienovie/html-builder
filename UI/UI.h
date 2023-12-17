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

namespace UI {
    int initialize();
    void cleanUp();
    void mainLoop();
    void showMainUI();
    void showTestWindow();
    void showSettingsWindow();
    bool mainLoopCondition();
    void glfw_error_callback(int error, const char* description);
    void setThemeByPath(const char* path);
    void saveThemeToFile(const char* name);
    void setAllThemeNames();
    std::string getThemeNameByPath(std::string themePath);
    void setThemeByName(std::string name, bool updateConfig = false);
    std::string getThemePathByName(std::string name);
    void assignCurrentThemeValueByName(std::string name);
}

