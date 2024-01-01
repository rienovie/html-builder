#pragma once

#include "../sys/util.h"
#include "ImGui/imgui.h"
#include "ImGui/imgui_stdlib.h"
#include "../sys/config.h"
#include "UI.h"
#include <map>
#include <string>

class win {
public:
    static void mainLoop();
private:
    static std::map<std::string,bool> mWindowBools;

    static void wMain();
    static void wSettings();
    static void wTest();
};
