#include "UI.h"

//required
void glfw_error_callback ( int error, const char* description ) {
    fprintf(stderr, "GLFW Error %d: %s\n",error,description);
}

/* Not sure if there's a better way to do this, tried initializing variables inside header
 * and the compiler said: "non-const static data member must be initialized out of line"
 * So I tried to set the values inside the constructor but then the compiler said:
 * "undefined reference to UI::nameOfVariable" so this seems to solve it but doesn't
 * feel right
*/

//VARIABLE INITIALIZATION
int
    UI::iFontSize = 32,
    UI::iCurrentTheme = 0,
    UI::iUserWindowWidth = 1600,
    UI::iUserWindowHeight = 900,
    UI::iLastStoredFontSize = 32;

ImVec2
    UI::limitFontSize = ImVec2(10,42),
    UI::limitScrollBarSize = ImVec2(1,24),
    UI::limitScrollBarRounding = ImVec2(0,12),
    UI::limitTabRounding = ImVec2(0,12),
    UI::limitFrameBorder = ImVec2(0,3),
    UI::limitFrameRounding = ImVec2(0,12),
    UI::limitFramePad = ImVec2(0,20),
    UI::limitItemSpacing = ImVec2(0,20),
    UI::limitSeparatorThickness = ImVec2(1,8),
    UI::limitCircleTess = ImVec2(0.10,5.00),
    UI::limitGrabMinSize = ImVec2(10,80);

ImFont
    *UI::font_main,
    *UI::font_bold,
    *UI::font_light;

ImVec4 UI::clearColor = ImVec4(0.45,0.55,0.60,1.00);
std::vector<std::string>
    UI::vFoundThemes,
    UI::vFavorites;
std::map<std::string,int> UI::mColorEnum;
bool
    UI::bDefaultThemeActive = false,
    UI::bFavoritesUpdated = true; //set to true so it'll run right away
ImGuiStyle *UI::uiStylePtr;
GLFWwindow *UI::mainWindow;

//CONSTRUCTOR
UI::UI() {
    glfwSetErrorCallback(glfw_error_callback);
    if(!glfwInit()) { throw std::string("glfw initialization error"); }

    //fallback if not found in config
    //config should have values updated to default for second run
    std::string sConvertCheck = "";
    sConvertCheck = config::getProp(config::system,"windowHeight");
    iUserWindowHeight = (sConvertCheck == "NULL") ? 480 : util::strToInt(sConvertCheck);
    sConvertCheck = config::getProp(config::system,"windowWidth");
    iUserWindowWidth = (sConvertCheck == "NULL") ? 640 : util::strToInt(sConvertCheck);

    //create window
    mainWindow = glfwCreateWindow( iUserWindowWidth,iUserWindowHeight,"Main Window",NULL,NULL);
    if(mainWindow == NULL) { throw std::string("mainWindow initialization error"); }
    glfwMakeContextCurrent(mainWindow);
    glfwSwapInterval(1); //vsync

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    font_main = io.Fonts->AddFontFromFileTTF("../fonts/Inconsolata-Medium.ttf",limitFontSize.y );
    font_bold = io.Fonts->AddFontFromFileTTF("../fonts/Inconsolata-ExtraBold.ttf",limitFontSize.y );
    font_light = io.Fonts->AddFontFromFileTTF("../fonts/Inconsolata-Light.ttf",limitFontSize.y );

    ImGui_ImplGlfw_InitForOpenGL(mainWindow,true);
    ImGui_ImplOpenGL3_Init("#version 130");

    uiStylePtr = &ImGui::GetStyle();

    setColorEnumMap();
    vFoundThemes = config::getAllThemeNames();
    iFontSize = util::strToInt(config::getProp(config::system,"fontSize"));
    assignCurrentThemeValueByName(config::getProp(config::system,"theme"));
    refreshTheme();

    //set manual settings here (shouldn't change by theme)
    uiStylePtr->WindowMenuButtonPosition = -1;

}

//DESTRUCTOR
UI::~UI() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(mainWindow);
    glfwTerminate();
}

//FUNCTIONS
bool UI::mainLoopCondition() {
    return !glfwWindowShouldClose(mainWindow);
}

void UI::mainLoop() {
    //poll and handle events (inputs, window resize, etc)
    glfwPollEvents();
    ImGui::GetIO().FontGlobalScale = float(iFontSize) / float(limitFontSize.y);

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    //windows main loop
    win::mainLoop();

    //rendering
    ImGui::Render();
    int display_w, display_h;
    glfwGetFramebufferSize(mainWindow, &display_w,&display_h);
    glViewport(0,0,display_w,display_h);
    glClearColor(clearColor.x * clearColor.w , clearColor.y * clearColor.w , clearColor.z * clearColor.w , clearColor.w);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(mainWindow);
}

void UI::tick_sec() {
    //update font config
    if(iLastStoredFontSize != iFontSize){
        iLastStoredFontSize = iFontSize;
        config::update(config::system,"fontSize",std::to_string(iFontSize));
    }
    if(bFavoritesUpdated) {
        vFavorites = config::getFavorites();
        bFavoritesUpdated = false;
    }
}

void UI::assignCurrentThemeValueByName ( std::string sName ) {
    bDefaultThemeActive = (sName == "Default");
    for(int i = 0;i<vFoundThemes.size();i++){
        if( vFoundThemes[i] == sName){
            iCurrentTheme = i;
            return;
        }
    }
}


void UI::setColorEnumMap() {
    for(int i = 0; i < ImGuiCol_COUNT; i++) {
        mColorEnum[ImGui::GetStyleColorName(i)] = i;
    }
}

void UI::refreshTheme() {
    uiStylePtr->FrameRounding = util::strToFloat(config::getProp(config::theme,"frameRounding"));
    uiStylePtr->GrabRounding = uiStylePtr->FrameRounding;
    uiStylePtr->PopupRounding = uiStylePtr->FrameRounding;
    uiStylePtr->ItemSpacing = getVec2FromString(config::getProp(config::theme,"itemSpacing"));
    uiStylePtr->ItemInnerSpacing = uiStylePtr->ItemSpacing;
    uiStylePtr->FramePadding = getVec2FromString(config::getProp(config::theme,"framePad"));
    uiStylePtr->TabRounding = util::strToFloat(config::getProp(config::theme,"tabRounding"));
    uiStylePtr->FrameBorderSize = util::strToFloat(config::getProp(config::theme,"frameBorder"));
    uiStylePtr->PopupBorderSize = uiStylePtr->FrameBorderSize;
    uiStylePtr->ScrollbarSize = util::strToFloat(config::getProp(config::theme,"scrollBarSize"));
    uiStylePtr->ScrollbarRounding = util::strToFloat(config::getProp(config::theme,"scrollBarRounding"));
    uiStylePtr->CircleTessellationMaxError = util::strToFloat(config::getProp(config::theme,"circleTess"));
    uiStylePtr->SeparatorTextBorderSize = util::strToFloat(config::getProp(config::theme,"separatorThickness"));
    uiStylePtr->WindowBorderSize = uiStylePtr->SeparatorTextBorderSize;
    uiStylePtr->GrabMinSize = util::strToFloat(config::getProp(config::theme,"grabMinSize"));

    auto mColorOptions = config::getAllThemeColorValues();

    for(auto colOpt : mColorOptions) {
        uiStylePtr->Colors[getColorEnum(colOpt.first)] = getVec4FromString(colOpt.second);
    }

    //make dockingEmptyColor darker
    //TODO need to make this update when user changes color
    //currently this only updates when changing theme
    ImVec4 modColor = uiStylePtr->Colors[ImGuiCol_WindowBg];
    float modVal = float(0.5);
    modColor.x *= modVal;
    modColor.y *= modVal;
    modColor.z *= modVal;
    modColor.w = 1;
    uiStylePtr->Colors[ImGuiCol_DockingEmptyBg] = modColor;
}

ImVec4 UI::getVec4FromString ( std::string sVec4Value ) {
    ImVec4 output = ImVec4(-1,-1,-1,-1);
    std::string sBuild = "";
    int iStrLength = sVec4Value.length();
    int iOutputCounter = 0; // 0,1,2,3 = x,y,z,w

    for(int i = 0; i < iStrLength; i++) {
        if(sVec4Value[i] == ',') {
            switch (iOutputCounter) {
                case 0:
                    output.x = util::strToFloat(sBuild);
                    break;
                case 1:
                    output.y = util::strToFloat(sBuild);
                    break;
                case 2:
                    output.z = util::strToFloat(sBuild);
                    break;
                default:
                    util::qPrint("Error iOutputCounter is out of bounds in getColorFromConfig!");
                    break;
            }
            sBuild.clear();
            iOutputCounter++;
        } else {
            sBuild.push_back(sVec4Value[i]);
        }
    }
    output.w = util::strToFloat(sBuild);

    if(output.x == -1 || output.y == -1 || output.z == -1 || output.z == -1) {
        util::qPrint("Error in getVec4FromString! Output =",output.x,output.y,output.z,output.w);
    }

    return output;
}

ImVec2 UI::getVec2FromString ( std::string sVec2Value ) {
    ImVec2 output = ImVec2(-1,-1);
    std::string sBuild = "";
    int iStringLength = sVec2Value.length();

    for(int i = 0; i < iStringLength; i++) {
        if(sVec2Value[i] == ',') {
            output.x = util::strToFloat(sBuild);
            output.y = util::strToFloat(sVec2Value.substr(i+1,iStringLength - i));
            break;
        } else {
            sBuild.push_back(sVec2Value[i]);
        }
    }

    if(output.x == -1 || output.y == -1) {
        util::qPrint("Error in getVec2FromString! Output =",output.x,output.y);
    }

    return output;
}

int UI::getColorEnum ( std::string sColorName ) {
    if(sColorName[0] != '~') { return -1; }
    sColorName.erase(0,1);

    return mColorEnum[sColorName];
}

std::string UI::getStringFromVec2 ( ImVec2 vec2Value ) {
    std::string sOutput = "";

    sOutput.append(std::to_string(vec2Value.x));
    sOutput.append(",");
    sOutput.append(std::to_string(vec2Value.y));

    return sOutput;
}

std::string UI::getStringFromVec4 ( ImVec4 vec4Value ) {
    std::string sOutput = "";

    sOutput.append(std::to_string(vec4Value.x));
    sOutput.append(",");
    sOutput.append(std::to_string(vec4Value.y));
    sOutput.append(",");
    sOutput.append(std::to_string(vec4Value.z));
    sOutput.append(",");
    sOutput.append(std::to_string(vec4Value.w));

    return sOutput;
}

void UI::exitApplication() {
    glfwSetWindowShouldClose(mainWindow,GLFW_TRUE);
}

void UI::createNewThemeAndSetCurrent ( std::string sName ) {
    vFoundThemes.push_back(sName);
    config::createNewThemeFromCurrent(sName);
    iCurrentTheme = util::searchVector(vFoundThemes,sName,true);
    bDefaultThemeActive = false;
}
