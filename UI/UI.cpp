#include "UI.h"

//settings
static int iMaxFontSize = 42;
static int iFontSize = 32;
static bool bDarkMode = true;
static int iCurrentTheme = 0; //is set to the combo selection
static int iUserWindowWidth = 1600;
static int iUserWindowHeight = 900;
static int iLastStoredFontSize = 32;

//windows
static bool bTestWindow_open = false;
static bool showDemo = false;
static bool showSettings = false;

std::vector<std::string> vFoundThemes;
std::map<std::string, int> mColorEnum;

ImFont *font_main , *font_bold , *font_light;
ImVec4 clearColor = ImVec4(0.45f,0.55f,0.60f,1.00f);
GLFWwindow* mainWindow;

namespace UI {
    int initialize() {
        glfwSetErrorCallback(glfw_error_callback);
        if (!glfwInit())
            return 1;

        iUserWindowHeight = util::strToInt(config::getProp(config::system,"windowHeight"));
        iUserWindowWidth = util::strToInt(config::getProp(config::system,"windowWidth"));

        //fallback if not found in config
        //config should have values updated to default for second run
        if( iUserWindowHeight == 0) {
        iUserWindowHeight = 480; }
        if( iUserWindowWidth == 0) {
        iUserWindowWidth = 640; }

        //create window
        mainWindow = glfwCreateWindow( iUserWindowWidth,iUserWindowHeight,"Main Window",NULL,NULL);
        if(mainWindow == NULL) { return 1; }
        glfwMakeContextCurrent(mainWindow);
        glfwSwapInterval(1); //vsync

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();

        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
        font_main = io.Fonts->AddFontFromFileTTF("../fonts/Inconsolata-Medium.ttf",iMaxFontSize );
        font_bold = io.Fonts->AddFontFromFileTTF("../fonts/Inconsolata-ExtraBold.ttf",iMaxFontSize );
        font_light = io.Fonts->AddFontFromFileTTF("../fonts/Inconsolata-Light.ttf",iMaxFontSize );

        ImGui_ImplGlfw_InitForOpenGL(mainWindow,true);
        ImGui_ImplOpenGL3_Init("#version 130");

        setColorEnumMap();
        vFoundThemes = config::getAllThemeNames();
        iFontSize = util::strToInt(config::getProp(config::system,"fontSize"));
        for(int i = 0;i<vFoundThemes.size();i++){
            if(vFoundThemes[i] == config::getProp(config::system,"theme")){
               iCurrentTheme = i;
            }
        }
        refreshTheme();

        return 0;

    }

    bool mainLoopCondition() {
        return !glfwWindowShouldClose(mainWindow);
    }


    void glfw_error_callback(int error, const char* description) {
        fprintf(stderr, "GLFW Error %d: %s\n", error, description);
    }

    void mainLoop(){
        //poll and handle events (inputs, window resize, etc)
        glfwPollEvents();
        ImGui::GetIO().FontGlobalScale = float(iFontSize) / float(iMaxFontSize);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        showMainUI();

        //windows
        if(showDemo) { ImGui::ShowDemoWindow(); }
        if( bTestWindow_open ) { showTestWindow(); }
        if(showSettings) { showSettingsWindow(); }

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

    void cleanUp(){
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
        glfwDestroyWindow(mainWindow);
        glfwTerminate();
    }

    void showMainUI(){
        ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());

        ImGui::Begin("Main",NULL,ImGuiWindowFlags_NoCollapse);
        if(ImGui::Button("Toggle test Window")) { bTestWindow_open = !bTestWindow_open; }
        if(ImGui::Button("Toggle Demo Window")) { showDemo = !showDemo; }
        if(ImGui::Button("Toggle Settings Window")) { showSettings = !showSettings; }
        ImGui::End();
    }

    void showTestWindow(){
        ImGui::Begin("Test Window",NULL);
        ImGui::PushFont(font_light);
        ImGui::Text("I'm a test window!");
        ImGui::PopFont();
        ImGui::PushFont(font_main);
        if(ImGui::Button("Close")) { bTestWindow_open = false; }
        ImGui::PopFont();
        ImGui::End();
    }

    void showSettingsWindow(){
        ImGui::Begin("Settings",NULL);

        if (ImGui::BeginCombo("Theme",vFoundThemes[iCurrentTheme].c_str())){
            //for each theme
            for(int i = 0;i<vFoundThemes.size();i++){
                const bool bSelected = (iCurrentTheme == i);
                if(ImGui::Selectable(vFoundThemes[i].c_str(),bSelected)) {
                    iCurrentTheme = i;
                    //selection change is here
                    config::update(config::system,"theme",vFoundThemes[i]);
                    refreshTheme();
                }
                if( bSelected ) { ImGui::SetItemDefaultFocus(); }
            }
            ImGui::EndCombo();
        }

        if(ImGui::Button("Default")) { iFontSize = 24; };
        ImGui::SameLine();
        ImGui::PushFont(font_bold);
        ImGui::SliderInt("Font Size",&iFontSize,10,iMaxFontSize);
        ImGui::PopFont();
        if(ImGui::Button("Toggle Dark Mode")) {
            bDarkMode = !bDarkMode;
            //if( bDarkMode ) { ImGui::StyleColorsDark(); }
            //else ImGui::StyleColorsLight();
            //TODO darkMode is being replaced, remove when fully updated
            config::update(config::theme,"darkMode",std::to_string(bDarkMode));

        } ;
        ImGui::End();
    }

    void assignCurrentThemeValueByName ( std::string name ) {
        for(int i = 0;i<vFoundThemes.size();i++){
            if( vFoundThemes[i] == name){
                iCurrentTheme = i;
                break;
            }
        }
    }

    //called each second in main tick_sec function
    void tick_sec() {
        //update font config
        if(iLastStoredFontSize != iFontSize){
            iLastStoredFontSize = iFontSize;
            config::update(config::system,"fontSize",std::to_string(iFontSize));
        }
    }

    void refreshTheme() {
        //feels like I should have the style outside this scope so I don't have
        //recreate the variable everytime I need to refreshTheme
        ImGuiStyle& uiStyle = ImGui::GetStyle();

        uiStyle.FrameRounding = util::strToFloat(config::getProp(config::theme,"frameRounding"));
        uiStyle.ItemSpacing = ImVec2(
            util::strToFloat(config::getProp(config::theme,"itemSpacingX")),
            util::strToFloat(config::getProp(config::theme,"itemSpacingY")));
        uiStyle.ItemInnerSpacing = ImVec2(
            util::strToFloat(config::getProp(config::theme,"itemSpacingX")),
            util::strToFloat(config::getProp(config::theme,"itemSpacingY")));
        uiStyle.FramePadding = ImVec2(
            util::strToFloat(config::getProp(config::theme,"framePadX")),
            util::strToFloat(config::getProp(config::theme,"framePadY")));
        uiStyle.TabRounding = util::strToFloat(config::getProp(config::theme,"tabRounding"));
        uiStyle.FrameBorderSize = util::strToFloat(config::getProp(config::theme,"frameBorder"));

        auto mColorOptions = config::getAllThemeColorValues();

        for(auto colOpt : mColorOptions) {
            uiStyle.Colors[getColorEnum(colOpt.first)] = getColorFromConfig(colOpt.second);
        }

    }

    ImVec4 getColorFromConfig (std::string sColorValue) {
        ImVec4 output = ImVec4(-1,-1,-1,-1);
        std::string sBuild = "";
        int iStrLength = sColorValue.length();
        int iOutputCounter = 0; // 0,1,2,3 = x,y,z,w

        for(int i = 0; i < iStrLength; i++) {
            if(sColorValue[i] == ',') {
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
                sBuild.push_back(sColorValue[i]);
            }
        }
        output.w = util::strToFloat(sBuild);

        return output;

    }

    int getColorEnum ( std::string sColorName ) {
        if(sColorName[0] != '~') { return -1; }
        sColorName.erase(0,1);

        return mColorEnum[sColorName];
    }

    void setColorEnumMap() {
        for(int i = 0; i < ImGuiCol_COUNT; i++) {
            mColorEnum[ImGui::GetStyleColorName(i)] = i;
        }
    }




}
