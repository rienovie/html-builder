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
#include "../sys/html.h"
#include "../sys/fetch.h"

void glfw_error_callback(int error, const char* description);

class UI {
public:

    struct colorHSV {
        float fHue, fSat, fValue;

        colorHSV() : fHue(0), fSat(0), fValue(0) {}
        colorHSV(float hue, float sat, float value) : fHue(hue), fSat(sat), fValue(value) {}
    };

    static int
        iFontSize,
        iCurrentTheme,
        iMaxRawLength;

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

    static std::vector<std::string>
        vFoundThemes,
        vFavorites;

    static ImFont
        *font_main,
        *font_bold,
        *font_light;

    static bool
        bDefaultThemeActive,
        bFullElementNames,
        bFavoritesUpdated;

    static ImGuiStyle* uiStylePtr;
    static ImGuiIO* ioPtr;
    static html::element* selectedElement;
    static std::string sItemSearch;
    static std::vector<ImGuiCol> vHueModValues;
    static std::map<std::string,ImVec4> mCustomColorProps;

    //functions
    static void
        mainLoop(),
        tick_sec(),
        assignCurrentThemeValueByName(std::string sName),
        refreshTheme(),
        exitApplication(),
        createNewThemeAndSetCurrent(std::string sName);

    static std::string
        getStringFromVec2(ImVec2 vec2Value),
        getStringFromVec4(ImVec4 vec4Value);

    static ImVec4 getVec4FromString(std::string sVec4Value);
    static ImVec2 getVec2FromString(std::string sVec2Value);
    static bool mainLoopCondition();
    static int getColorEnum(std::string sColorName);
    static colorHSV getCurrentColorAsHSV(ImGuiCol eColor);

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

